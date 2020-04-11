//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"
#include "../Lib/DecodePackage.hpp"
#include "../../../Utilities/time.hpp"

namespace filter {
    using namespace si;
    using namespace si::literals;

    constexpr auto ACC_SIGMA_V = 10 * si::acceleration / si::second; // Process noise
    constexpr auto ACC_SIGMA_W = 0.035F * si::acceleration; // Measurement noise
    constexpr auto CALIB_STAT_THRESH = 0;

    Fusion::Fusion() : lastUpdate{util::time::get()}, accXFilter{ACC_SIGMA_V, ACC_SIGMA_W},
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
            //std::cout << "No PDB Package received!" << std::endl;
        }

        if (lastTaranisPackage.has_value()) {
            res.taranisPackage = fusion::decodePackage<TaranisPackage>(lastTaranisPackage.value());
        } else {
            //std::cout << "No Taranis Package received!" << std::endl;
        }

        if (lastRemotePackage.has_value()) {
            res.loraRemote = fusion::decodePackage<LoraPackage>(lastRemotePackage.value());
        } else {
            //std::cout << "No Lora-Remote Package received!" << std::endl;
        }

        if (!lastNavPackage.has_value()) {
            std::cerr << "No Nav Package received!" << std::endl;
            return;
        }

        if (!lastGpsMeasurement.has_value()) {
            std::cerr << "No GPS Package received!" << std::endl;
            return;
        }


        auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);
        auto gpsMeasurement = lastGpsMeasurement.value();
        auto navData = fusion::decodePackage<NavPackage>(lastNavPackage.value());

        const auto startTime = util::time::get();
        const auto dtDouble = startTime - lastUpdate;
        const auto dt = static_cast<si::Second<si::default_type>>(dtDouble);
        lastUpdate = startTime;

        if (!calibration.isCalibrated()) {
            if (gpsMeasurement.fixAquired
                && flightControllerData.sysCalibStatus >= CALIB_STAT_THRESH &&
                flightControllerData.accCalibStatus >= CALIB_STAT_THRESH
                && flightControllerData.magCalibStatus >= CALIB_STAT_THRESH &&
                flightControllerData.gyroCalibStatus >= CALIB_STAT_THRESH) {
                calibration.update(startTime, flightControllerData, gpsMeasurement, navData);
            } else {
                std::cout << "Calib not running, reason(s):\n";

                if (!gpsMeasurement.fixAquired) {
                    std::cerr << "\tNo GPS Fix" << std::endl;
                }
                if (flightControllerData.sysCalibStatus < CALIB_STAT_THRESH) {
                    std::cerr << "\tBNO Sys Calib Stat: " << static_cast<int>(flightControllerData.sysCalibStatus)
                              << std::endl;
                }
                if (flightControllerData.magCalibStatus < CALIB_STAT_THRESH) {
                    std::cerr << "\tBNO Mag Calib Stat: " << static_cast<int>(flightControllerData.magCalibStatus)
                              << std::endl;
                }
                if (flightControllerData.accCalibStatus < CALIB_STAT_THRESH) {
                    std::cerr << "\tBNO Acc Calib Stat: " << static_cast<int>(flightControllerData.accCalibStatus)
                              << std::endl;
                }
            }
        } else {
            if (!gpsMeasurement.fixAquired) {
                std::cerr << "No GPS fix, running in pure prediction mode" << std::endl;
            }

            if (flightControllerData.sysCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Sys Calib Stat: " << static_cast<int>(flightControllerData.sysCalibStatus)
                          << std::endl;
            }
            if (flightControllerData.magCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Mag Calib Stat: " << static_cast<int>(flightControllerData.magCalibStatus)
                          << std::endl;
            }
            if (flightControllerData.accCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Acc Calib Stat: " << static_cast<int>(flightControllerData.accCalibStatus)
                          << std::endl;
            }
            if (flightControllerData.gyroCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Gyro Calib Stat: " << static_cast<int>(flightControllerData.gyroCalibStatus)
                          << std::endl;
            }

            calibration.applyCalib(startTime, flightControllerData, gpsMeasurement, navData);


            auto state = particleFilter.update(dt, flightControllerData, gpsMeasurement,
                                               navData, calibration.getAdditionalBaroUncertainty());

            lastGpsMeasurement->location.lat = NAN;
            lastGpsMeasurement->location.lon = NAN;
            lastGpsMeasurement->location.altitude = NAN * meter;
            lastGpsMeasurement->speed = NAN * speed;
            lastGpsMeasurement->climb = NAN * speed;

            accXFilter.addMeasurement(flightControllerData.accX, dt);
            accYFilter.addMeasurement(flightControllerData.accY, dt);
            accZFilter.addMeasurement(flightControllerData.accZ, dt);

            res.roll = state.roll_angle;
            res.rollDeriv = state.roll_deriv * hertz;
            res.pitch = state.pitch_angle;
            res.pitchDeriv = state.pitch_deriv * hertz;
            res.yaw = state.yaw_angle;
            res.yawDeriv = state.yaw_deriv * hertz;
            res.speed = state.speed * speed;
            res.position.altitude = state.altitude * meter;
            res.altitudeAboveGround = state.altitude_above_ground * meter;
            res.altitudeGround = res.position.altitude - res.altitudeAboveGround;
            res.position.lat = state.lat;
            res.position.lon = state.lon;
            res.accX = accXFilter.getMeasurementEstimate();
            res.accY = accYFilter.getMeasurementEstimate();
            res.accZ = accZFilter.getMeasurementEstimate();
            res.startTime = calibration.getStartTime();
            res.startLocation = calibration.getStartLocation();

            res.rawFlightControllerData = flightControllerData;
            res.navPackage = navData;

            std::cout << "dt=" << dt << "\truntime=" << util::time::get() - startTime << std::endl;

            out.put(res);
        }
    }
}
