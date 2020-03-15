//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"
#include "../Lib/DecodePackage.hpp"

namespace filter {
    using namespace si::extended;
    using namespace si::literals;

    Fusion::Fusion() {
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
            gpsIn.get(lastGpsMeasurement);
            pdbIn.get(lastPdbPackage);
            taranisIn.get(lastTaranisPackage);
            baseIn.get(lastBasePackage);
            remoteIn.get(lastRemotePackage);
            ultrasonicIn.get(lastUltrasonicDistance);

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
            auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);
            auto state  = particleFilter.update(flightControllerData, lastGpsMeasurement.value(),
                    lastUltrasonicDistance.value());

            res.roll = state.roll_angle;
            res.pitch = state.pitch_angle;
            res.yaw = state.yaw_angle;
            res.speed = state.speed * speed;
            res.altitude = state.altitude * meter;
            res.altitudeAboveGround = state.altitude_above_ground * meter;
            res.lat = state.lat;
            res.lon = state.lon;

            out.put(res);
        }
    }
}
