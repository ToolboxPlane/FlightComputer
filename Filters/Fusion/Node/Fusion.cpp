//
// Created by paul on 14.03.18.
//

#include <numeric>
#include "Fusion.hpp"
#include "../Lib/DecodePackage.hpp"

namespace filter {
    using namespace si::extended;
    using namespace si::literals;

    constexpr auto ACC_SIGMA_V = 1900 * si::extended::acceleration / si::base::second; // Process noise
    constexpr auto ACC_SIGMA_W = 0.035F * si::extended::acceleration; // Measurement noise

    Fusion::Fusion() : lastUpdate{getCurrSeconds<long double>()}, accXFilter{ACC_SIGMA_V, ACC_SIGMA_W},
                       accYFilter{ACC_SIGMA_V, ACC_SIGMA_W}, accZFilter{ACC_SIGMA_V, ACC_SIGMA_W} {
        this->start();
    }

    OutputChannel<State_t> &Fusion::getChannelOut() {
        return out;
    }

    InputChannel<GpsMeasurement_t> &Fusion::getGpsIn() {
        return gpsIn;
    }

    InputChannel<rcLib::Package> &Fusion::getFlightControllerIn() {
        return flightControllerIn;
    }

    InputChannel<rcLib::Package> &Fusion::getBaseIn() {
        return baseIn;
    }

    InputChannel<rcLib::Package> &Fusion::getRemoteIn() {
        return remoteIn;
    }

    InputChannel<rcLib::Package> &Fusion::getPdbIn() {
        return pdbIn;
    }

    InputChannel<rcLib::Package> &Fusion::getTaranisIn() {
        return taranisIn;
    }

    InputChannel<rcLib::Package> &Fusion::getNavIn() {
        return navIn;
    }

    void Fusion::run() {
        while (!flightControllerIn.isClosed()) {
            while (gpsIn.get(lastGpsMeasurement));
            while (pdbIn.get(lastPdbPackage));
            while (taranisIn.get(lastTaranisPackage));
            while (baseIn.get(lastBasePackage));
            while (remoteIn.get(lastRemotePackage));
            while (navIn.get(lastNavPackage));

            do {
                flightControllerIn.get(lastFcPackage);
            } while (flightControllerIn.numAvailable() > 0);

            process();
        }

        std::exit(0);
    }

    void Fusion::process() {
        State_t res{};

        if (lastPdbPackage.has_value()) {
            res.pdbPackage = fusion::decodePackage<PdbPackage>(lastPdbPackage.value());
        } else {
            std::cout << "No PDB Package received!" << std::endl;
        }

        if (lastTaranisPackage.has_value()) {
            res.taranisPackage = fusion::decodePackage<TaranisPackage>(lastTaranisPackage.value());
        } else {
            std::cout << "No Taranis Package received!" << std::endl;
        }

        if (lastRemotePackage.has_value()) {
            res.loraRemote = fusion::decodePackage<LoraPackage>(lastRemotePackage.value());
        } else {
            std::cout << "No Lora-Remote Package received!" << std::endl;
        }

        if (lastGpsMeasurement.has_value() && lastGpsMeasurement.value().fixAquired && lastNavPackage.has_value()) {
            const auto startTime = getCurrSeconds<long double>();
            auto navData = fusion::decodePackage<NavPackage>(lastNavPackage.value());
            auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);
            auto gpsMeasurement = lastGpsMeasurement.value();

            if (!calibration.isCalibrated()) {
                calibration.update(startTime, flightControllerData, gpsMeasurement, navData);
                return;
            }
            calibration.applyCalib(startTime, flightControllerData, gpsMeasurement, navData);

            const auto dtDouble = startTime - lastUpdate;
            const auto dt = static_cast<si::base::Second<si::default_type>>(dtDouble);
            lastUpdate = startTime;

            auto state = particleFilter.update(dt, flightControllerData, gpsMeasurement,
                                               navData, calibration.getAdditionalBaroUncertainty());

            accXFilter.addMeasurement(flightControllerData.accX, dt);
            accYFilter.addMeasurement(flightControllerData.accY, dt);
            accZFilter.addMeasurement(flightControllerData.accZ, dt);

            res.roll = state.roll_angle;
            res.pitch = state.pitch_angle;
            res.yaw = state.yaw_angle;
            res.speed = state.speed * speed;
            res.altitude = state.altitude * meter;
            res.altitudeAboveGround = state.altitude_above_ground * meter;
            res.altitudeGround = res.altitude - res.altitudeAboveGround;
            res.lat = state.lat;
            res.lon = state.lon;
            res.accX = accXFilter.getMeasurementEstimate();
            res.accY = accYFilter.getMeasurementEstimate();
            res.accZ = accZFilter.getMeasurementEstimate();
            res.startTime = calibration.getStartTime();
            res.startLocation = calibration.getStartLocation();

            res.rawFlightControllerData = flightControllerData;
            res.navPackage = navData;

            //std::cout << getCurrSeconds<long double>() - startTime << std::endl;

            out.put(res);
        } else {
            std::cerr << "Fusion not running, reason(s):\n";
            if (!lastGpsMeasurement.has_value()) {
                std::cerr << "\tNo GPS Measurement" << std::endl;
            } else if (!lastGpsMeasurement.value().fixAquired) {
                std::cerr << "\tNo GPS Fix" << std::endl;
            }

            if (!lastNavPackage.has_value()) {
                std::cerr << "\tNo Nav Data" << std::endl;
            }
        }
    }

    template<typename T>
    auto Fusion::getCurrSeconds() -> si::base::Second<T> {
        auto tp = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto microseconds = static_cast<T>(
                std::chrono::duration_cast<std::chrono::microseconds>(tp).count());
        return si::base::Second<T>(microseconds / 10e6);
    }
}
