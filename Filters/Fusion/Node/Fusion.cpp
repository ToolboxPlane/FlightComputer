//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "Fusion.hpp"
#include "../Type/State_t.hpp"
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

    void Fusion::run() {
        while (!flightControllerIn.isClosed()) {
            lastGpsMeasurement = gpsIn.get();
            lastPdbPackage = pdbIn.get();
            lastTaranisPackage = taranisIn.get();
            lastBasePackage = baseIn.get();
            lastRemotePackage = remoteIn.get();

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

        // Flightcontroller Data
        auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);

        // Gps
        if (lastGpsMeasurement.has_value() && lastGpsMeasurement.value().fixAquired) {
            res.position = lastGpsMeasurement.value();
            if (lastState.position.timestamp < res.position.timestamp) {
                res.groundSpeed = res.position.location.distanceTo(lastState.position.location) /
                        (res.position.timestamp - lastState.position.timestamp);
                //@TODO maybe res.position.speed is the better choice, requires testing
            } else {
                res.groundSpeed = lastState.groundSpeed;
            }
        } else {
            res.groundSpeed = res.airspeed;
            res.position.fixAquired = false;
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
