//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "Fusion.hpp"

#define LAST_VALUES_LENGTH 64

void Fusion::run() {
    rcLib::PackageExtended lastSerialPackage;
    Gps_t lastGps;
    ProcessingStatus flightControllerStatus = NOT_RECEIVED, gpsStatus = NOT_RECEIVED;
    while(!out.isClosed()) {
         if(!flightControllerIn.isClosed() && flightControllerIn.get(lastSerialPackage, false)) {
            flightControllerStatus = READY;

            lastFlightControllerPackages.push_back(lastSerialPackage);
            if(lastFlightControllerPackages.size() >= LAST_VALUES_LENGTH) {
                lastFlightControllerPackages.pop_front();
            }
        } else if(!gpsIn.isClosed() && gpsIn.get(lastGps, false)) {
            gpsStatus = READY;

            lastGpsValues.push_back(lastGps);
            if(lastGpsValues.size() >= LAST_VALUES_LENGTH) {
                lastGpsValues.pop_front();
            }
        }

        if(flightControllerStatus == READY || gpsStatus == READY) {
            if(flightControllerStatus != NOT_RECEIVED && gpsStatus != NOT_RECEIVED) {
                out.put(process());
                flightControllerStatus = PROCESSED;
                gpsStatus = PROCESSED;
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

    res.position = lastGpsValues.back();
    if(res.position.timestamp != lastState.position.timestamp) {
        res.groundSpeed = res.position.distanceTo(lastState.position) /
                          (res.position.timestamp - lastState.position.timestamp);
    } else {
        res.groundSpeed = lastState.groundSpeed;
    }

    res.airspeed = res.groundSpeed;

    res.heading = 0;
    res.roll = 0;
    res.pitch = 0;
    res.heightAboveGround = 0;
    res.heightAboveSeaLevel = 0;
    res.voltage = 0;

    auto c = 0;
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
        res.voltage += iterator->getChannel(13) / 10.0 * weight;
    }

    res.heightAboveGround = (res.heightAboveSeaLevel + lastGpsValues.back().altitude*0)/1;


    lastState = res;

    return res;
}

Channel<State_t> &Fusion::getChannelOut() {
    return out;
}

Channel<Gps_t> &Fusion::getGpsIn() {
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
