//
// Created by paul on 14.03.18.
//

#include <numeric>
#include "Fusion.hpp"
#include "../Lib/DecodePackage.hpp"

namespace filter {
    using namespace si::extended;
    using namespace si::literals;

    constexpr auto ACC_SIGMA_V = 400 * si::extended::acceleration / si::base::second;
    constexpr auto ACC_SIGMA_W = 0.0135 * si::extended::acceleration; // Measurement noise

    Fusion::Fusion() : lastUpdate{getCurrSeconds()}, accXFilter{ACC_SIGMA_V, ACC_SIGMA_W},
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

    InputChannel<si::base::Meter<>> &Fusion::getUltrasonicIn() {
        return ultrasonicIn;
    }

    void Fusion::run() {
        while (!flightControllerIn.isClosed()) {
            while (gpsIn.get(lastGpsMeasurement));
            while (pdbIn.get(lastPdbPackage));
            while (taranisIn.get(lastTaranisPackage));
            while (baseIn.get(lastBasePackage));
            while (remoteIn.get(lastRemotePackage));
            while (ultrasonicIn.get(lastUltrasonicDistance));

            if (flightControllerIn.get(lastFcPackage)) {
                process();
            } else {
                std::this_thread::yield();
            }
        }
    }

    void Fusion::process() {
        State_t res{};

        if (lastPdbPackage.has_value()) {
            res.pdbPackage = fusion::decodePackage<PdbPackage>(lastPdbPackage.value());
        }

        if (lastTaranisPackage.has_value()) {
            res.taranisPackage = fusion::decodePackage<TaranisPackage>(lastTaranisPackage.value());
        }

        if (lastRemotePackage.has_value()) {
            res.loraRemote = fusion::decodePackage<LoraPackage>(lastRemotePackage.value());
        }

        if (lastGpsMeasurement.has_value() && lastGpsMeasurement.value().fixAquired && lastUltrasonicDistance.has_value()) {
            const auto startTime = getCurrSeconds();
            const auto dt = startTime - lastUpdate;
            lastUpdate = startTime;

            auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);
            auto state  = particleFilter.update(dt, flightControllerData, lastGpsMeasurement.value(),
                    lastUltrasonicDistance.value());

            accXFilter.addMeasurement(flightControllerData.accX, dt);
            accYFilter.addMeasurement(flightControllerData.accY, dt);
            accZFilter.addMeasurement(flightControllerData.accZ, dt);

            res.roll = state.roll_angle;
            res.pitch = state.pitch_angle;
            res.yaw = state.yaw_angle;
            res.speed = state.speed * speed;
            res.altitude = state.altitude * meter;
            res.altitudeAboveGround = state.altitude_above_ground * meter;
            res.lat = state.lat;
            res.lon = state.lon;
            res.accX = accXFilter.getMeasurementEstimate();
            res.accY = accYFilter.getMeasurementEstimate();
            res.accZ = accZFilter.getMeasurementEstimate();

            out.put(res);
        }
    }

    auto Fusion::getCurrSeconds() -> si::base::Second<> {
        auto tp = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto microseconds = static_cast<long double>(
                std::chrono::duration_cast<std::chrono::microseconds>(tp).count());
        return microseconds / 10e6 * si::base::second;
    }
}
