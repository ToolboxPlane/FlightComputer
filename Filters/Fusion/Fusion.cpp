//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "Fusion.hpp"
#include "State_t.hpp"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

Fusion::Fusion() :
        lastPdbPackage(std::experimental::nullopt), lastGpsMeasurement(std::experimental::nullopt),
        lastBasePackage(std::experimental::nullopt), lastTaranisPackage(std::experimental::nullopt)
{
    this->start();
}

MultipleOutputChannel<State_t> &Fusion::getChannelOut() {
    return out;
}

Channel<GpsMeasurement_t> &Fusion::getGpsIn() {
    return gpsIn;
}

Channel<rcLib::PackageExtended> &Fusion::getFlightControllerIn() {
    return flightControllerIn;
}

Channel<rcLib::PackageExtended> &Fusion::getBaseIn() {
    return baseIn;
}

Channel<rcLib::PackageExtended> &Fusion::getRemoteIn() {
    return remoteIn;
}

Channel<rcLib::PackageExtended> &Fusion::getPdbIn() {
    return pdbIn;
}


Channel<rcLib::PackageExtended> &Fusion::getTaranisIn() {
    return taranisIn;
}

void Fusion::run() {

    while(true) {
        if (!gpsIn.isClosed()) {
            gpsIn.get(*lastGpsMeasurement, false);
        }
        if (!pdbIn.isClosed()) {
            pdbIn.get(*lastPdbPackage, false);
        }
        if(!taranisIn.isClosed()) {
            taranisIn.get(*lastTaranisPackage, false);
        }
        if(!baseIn.isClosed()) {
            baseIn.get(*lastBasePackage, false);
        }
        if(!remoteIn.isClosed()) {
            remoteIn.get(*lastRemotePackage, false);
        }
        if (!flightControllerIn.isClosed() && flightControllerIn.get(lastFcPackage, false)) {
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
    res.airspeed = res.groundSpeed;

    res.heading = lastFcPackage.getChannel(0);
    res.roll = lastFcPackage.getChannel(1);
    res.pitch = lastFcPackage.getChannel(2);
    res.heightAboveSeaLevel = lastFcPackage.getChannel(4);

    /*auto c = 0;
    for(auto iterator = lastFlightControllerPackages.begin();
            iterator != lastFlightControllerPackages.end(); iterator++, c++) {
        double weight = std::pow(0.5, (lastFlightControllerPackages.size() - c));
        // Little workaround to get the sum of all c's to be 1
        if(c == 0) {
            weight *= 2;
        }

        res.heading += iterator->getChannel(0) * weight;
        res.roll += (iterator->getChannel(1) - 180) * weight;
        res.pitch += (iterator->getChannel(2) - 180) * weight;
        res.heightAboveGround += 0;
        res.heightAboveSeaLevel += iterator->getChannel(4) * weight;
    }*/
    res.heightAboveGround = res.heightAboveSeaLevel; // Waiting for some kind of distance sensor

    // Gps
    if(lastGpsMeasurement) {
        res.position = *lastGpsMeasurement;
        if (res.position.timestamp != lastState.position.timestamp) {
            res.groundSpeed = res.position.location.distanceTo(lastState.position.location) /
                              (res.position.timestamp - lastState.position.timestamp);
        } else {
            res.groundSpeed = lastState.groundSpeed;
        }
    }

    // PDB
    if(lastPdbPackage) {
        res.voltage = (lastPdbPackage->getChannel(1) * 128) / 1000.0;
    }

    // Taranis
    if(lastTaranisPackage) {
        res.taranis.throttle = lastTaranisPackage->getChannel(6);
        res.taranis.yaw = lastTaranisPackage->getChannel(7);
        res.taranis.pitch = lastTaranisPackage->getChannel(8);
        res.taranis.roll= lastTaranisPackage->getChannel(9);
        res.taranis.isArmed = lastTaranisPackage->getChannel(10) != 0;
        res.taranis.manualOverrideActive = lastTaranisPackage->getChannel(11) != 0;
    }

    //@TODO handle remote and base, discuss which data to send

    lastState = res;

    return res;
}


#pragma clang diagnostic pop