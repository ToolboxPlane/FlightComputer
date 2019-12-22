//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"
#include "../Lib/DecodePackage.hpp"

namespace filter {
    using namespace si::extended;
    using namespace si::literals;

    Fusion::Fusion() : particleFilter{1000} {
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

    void Fusion::run() {
        while (!flightControllerIn.isClosed()) {
            gpsIn.get(lastGpsMeasurement);
            pdbIn.get(lastPdbPackage);
            taranisIn.get(lastTaranisPackage);
            baseIn.get(lastBasePackage);
            remoteIn.get(lastRemotePackage);

            if (flightControllerIn.get(lastFcPackage)) {
                out.put(process());
            } else {
                std::this_thread::yield();
            }
        }
    }

    State_t Fusion::process() {
        State_t res{};
        static State_t lastState;

        // Gps
        if (lastGpsMeasurement.has_value() && lastGpsMeasurement.value().fixAquired) {
            auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);
            auto [state, likelihood] = particleFilter.update(flightControllerData, lastGpsMeasurement.value());

            res.roll = state.roll_angle;
            res.rollDiff = state.roll_rate * hertz;
            res.pitch = state.pitch_angle;
            res.pitchDiff = state.pitch_rate * hertz;
            res.yaw = state.yaw_angle;
            res.yawDiff = state.yaw_rate * hertz;
            res.speed = state.speed * speed;
            res.altitude = state.altitude * meter;
            res.altitudeAboveGround = state.altitude_above_ground * meter;
            res.lat = state.lat;
            res.lon = state.lon;
        }

        if (lastPdbPackage.has_value()) {
            res.pdbPackage = fusion::decodePackage<PdbPackage>(lastPdbPackage.value());
        }

        if (lastTaranisPackage.has_value()) {
            res.taranisPackage = fusion::decodePackage<TaranisPackage>(lastTaranisPackage.value());
        }

        if (lastRemotePackage.has_value()) {
            res.loraRemote = fusion::decodePackage<LoraPackage>(lastRemotePackage.value());
        }

        //@TODO handle base, discuss which data to send. ATM not necessary due to non functioning lora tx

        lastState = res;

        return res;
    }

}
