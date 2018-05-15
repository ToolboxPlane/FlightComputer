//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "Fusion.hpp"


void Fusion::run() {
    ProcessingStatus flightControllerStatus = NOT_RECEIVED, gpsStatus = NOT_RECEIVED, powerStatus = NOT_RECEIVED;


    while(!out.isClosed()) {
        if (!flightControllerIn.isClosed() && flightControllerIn.get(fcPackage, false)) {
            flightControllerStatus = READY;
        }
        if (!gpsIn.isClosed() && gpsIn.get(gpsValue, false)) {
            gpsStatus = READY;

        }
        if (!pdbIn.isClosed() && pdbIn.get(pdbPackage, false)) {
             powerStatus = READY;
        }

        if(flightControllerStatus == READY || gpsStatus == READY || powerStatus == READY) {
            if(flightControllerStatus != NOT_RECEIVED && gpsStatus != NOT_RECEIVED && powerStatus != NOT_RECEIVED) {
                out.put(process());
                flightControllerStatus = PROCESSED;
                gpsStatus = PROCESSED;
                powerStatus = PROCESSED;
            } else {
                // Maybe do other stuff
            }
        } else {
            std::this_thread::yield();
        }
    }
}

State_t Fusion::process() {
    State_t res{};
    static State_t lastState;

    res.position = gpsValue;
    if(res.position.timestamp != lastState.position.timestamp) {
        res.groundSpeed = res.position.location.distanceTo(lastState.position.location) /
                          (res.position.timestamp - lastState.position.timestamp);
    } else {
        res.groundSpeed = lastState.groundSpeed;
    }

    res.airspeed = res.groundSpeed;

    res.heading = fcPackage.getChannel(0);
    res.roll = fcPackage.getChannel(1);
    res.pitch = fcPackage.getChannel(2);
    res.heightAboveSeaLevel = fcPackage.getChannel(4);

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

    res.voltage = (pdbPackage.getChannel(1) * 128)/1000.0;

    res.heightAboveSeaLevel = (res.heightAboveSeaLevel + gpsValue.location.altitude*0)/1;
    res.heightAboveGround = res.heightAboveSeaLevel;

    lastState = res;

    return res;
}

Channel<State_t> &Fusion::getChannelOut() {
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
