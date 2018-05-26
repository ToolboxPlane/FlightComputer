//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "Fusion.hpp"
#include "State_t.hpp"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

Fusion::Fusion()
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
            gpsIn.get(lastGpsMeasurement, false);
            gpsRecv = true;
        }
        if (!pdbIn.isClosed()) {
            pdbIn.get(lastPdbPackage, false);
            pdbRecv = true;
        }
        if(!taranisIn.isClosed()) {
            taranisIn.get(lastTaranisPackage, false);
            taranisRecv = true;
        }
        if(!baseIn.isClosed()) {
            baseIn.get(lastBasePackage, false);
            baseRecv = true;
        }
        if(!remoteIn.isClosed()) {
            remoteIn.get(lastRemotePackage, false);
            remoteRecv = true;
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

    res.heading = lastFcPackage.getChannel(0);
    res.roll = lastFcPackage.getChannel(1) - 180;
    res.pitch = lastFcPackage.getChannel(2) - 180;
    res.heightAboveSeaLevel = lastFcPackage.getChannel(4);
    res.airspeed = lastFcPackage.getChannel(6);

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
    if(gpsRecv) {
        res.position = lastGpsMeasurement;
        if (res.position.timestamp != lastState.position.timestamp) {
            res.groundSpeed = res.position.location.distanceTo(lastState.position.location) /
                              (res.position.timestamp - lastState.position.timestamp);
        } else {
            res.groundSpeed = lastState.groundSpeed;
        }
    }

    // PDB
    if(pdbRecv) {
        res.voltage = (lastPdbPackage.getChannel(1) * 128) / 1000.0;
    }

    // Taranis
    if(taranisRecv) {
        res.taranis.throttle = normalizeTaranis(lastTaranisPackage.getChannel(6));
        res.taranis.yaw = normalizeTaranis(lastTaranisPackage.getChannel(7));
        res.taranis.pitch = normalizeTaranis(lastTaranisPackage.getChannel(8));
        res.taranis.roll= normalizeTaranis(lastTaranisPackage.getChannel(9));
        res.taranis.isArmed = normalizeTaranis(lastTaranisPackage.getChannel(10)) > 250;
        res.taranis.manualOverrideActive = normalizeTaranis(lastTaranisPackage.getChannel(11)) > 250;
    }

    //@TODO handle remote and base, discuss which data to send

    lastState = res;

    return res;
}

int Fusion::normalizeTaranis(int input) {
    return (input-172)*1000/(1811-172);
}


#pragma clang diagnostic pop