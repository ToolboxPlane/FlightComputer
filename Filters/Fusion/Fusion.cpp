//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "Fusion.hpp"
#include "State_t.hpp"

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

    InputChannel<rcLib::PackageExtended> &Fusion::getFlightControllerIn() {
        return flightControllerIn;
    }

    InputChannel<rcLib::PackageExtended> &Fusion::getBaseIn() {
        return baseIn;
    }

    InputChannel<rcLib::PackageExtended> &Fusion::getRemoteIn() {
        return remoteIn;
    }

    InputChannel<rcLib::PackageExtended> &Fusion::getPdbIn() {
        return pdbIn;
    }

    InputChannel<rcLib::PackageExtended> &Fusion::getTaranisIn() {
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
        res.heading = lastFcPackage.getChannel(0);
        res.roll = lastFcPackage.getChannel(1) - 180;
        res.pitch = lastFcPackage.getChannel(2) - 180;
        res.heightAboveSeaLevel = lastFcPackage.getChannel(4) * si::base::meter;
        res.heightAboveGround = res.heightAboveSeaLevel; // Waiting for some kind of distance sensor
        res.airspeed = lastFcPackage.getChannel(5) * si::extended::speed;
        res.accForward = (lastFcPackage.getChannel(6) - 500) * si::extended::acceleration;
        res.accSide = (lastFcPackage.getChannel(7) - 500) * si::extended::acceleration;
        res.accUpdown = (lastFcPackage.getChannel(8) - 500) * si::extended::acceleration;

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

        // PDB
        if (lastPdbPackage.has_value()) {
            res.voltage = (lastPdbPackage.value().getChannel(1) * 128) / 1000.0 * si::extended::volt;
        } else {
            res.voltage = 16.8_volt;
        }

        // Taranis
        if (lastTaranisPackage.has_value()) {
            auto pkg = lastTaranisPackage.value();
            res.taranis.throttle = normalizeTaranis(pkg.getChannel(5));
            res.taranis.yaw = normalizeTaranis(pkg.getChannel(6));
            res.taranis.pitch = normalizeTaranis(pkg.getChannel(7));
            res.taranis.roll = normalizeTaranis(pkg.getChannel(8));
            res.taranis.isArmed = normalizeTaranis(pkg.getChannel(9)) > 250;
            res.taranis.manualOverrideActive = normalizeTaranis(pkg.getChannel(10)) < 250;
        }

        if (lastRemotePackage.has_value()) {
            auto pkg = lastRemotePackage.value();
            res.lora.joyRight.x = (pkg.getChannel(0) - 127) / 127.0;
            res.lora.joyRight.y = (pkg.getChannel(1) - 127) / 127.0;
            res.lora.joyLeft.x = (pkg.getChannel(2) - 127) / 127.0;
            res.lora.joyLeft.y = (pkg.getChannel(3) - 127) / 127.0;
            res.lora.flightMode = static_cast<FlightMode>(pkg.getChannel(4));
            res.lora.isArmed = static_cast<bool>(pkg.getChannel(5));
        } else {
            res.lora.flightMode = FlightMode::HOLD;
        }

        //@TODO handle base, discuss which data to send. ATM not necessary due to non functioning lora tx

        lastState = res;

        return res;
    }

    int Fusion::normalizeTaranis(int input) {
        return (input - 172) * 1000 / (1811 - 172);
    }
}
