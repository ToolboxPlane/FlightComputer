//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"

#define LAST_VALUES_LENGTH 64

void Fusion::run() {
    rcLib::Package lastSerialPackage;
    Gps_t lastGps{};
    ProcessingStatus serialStatus = NOT_RECEIVED, loraStatus = NOT_RECEIVED, gpsStatus = NOT_RECEIVED;
    while(!out.isClosed()) {
        if(!loraIn.isClosed() && loraIn.get(lastLoRaPackage, false)) {
            loraStatus = READY;
        } else if(!serialIn.isClosed() && serialIn.get(lastSerialPackage, false)) {
            serialStatus = READY;

            lastSerialPackages.push_back(lastSerialPackage);
            if(lastSerialPackages.size() >= LAST_VALUES_LENGTH) {
                lastSerialPackages.pop_front();
            }
        } else if(!gpsIn.isClosed() && gpsIn.get(lastGps, false)) {
            gpsStatus = READY;

            lastGpsValues.push_back(lastGps);
            if(lastGpsValues.size() >= LAST_VALUES_LENGTH) {
                lastGpsValues.pop_front();
            }
        }

        if(loraStatus == READY || serialStatus == READY || gpsStatus == READY) {
            if(loraStatus != NOT_RECEIVED && serialStatus != NOT_RECEIVED && gpsStatus != NOT_RECEIVED) {
                out.put(process());
                loraStatus = PROCESSED;
                serialStatus = PROCESSED;
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
    res.groundSpeed = res.position.distanceTo(lastState.position) /
            (res.position.timestamp - lastState.position.timestamp);

    res.airspeed = res.groundSpeed;

    res.heading = 0;
    res.roll = 0;
    res.pitch = 0;
    res.heightAboveGround = 0;
    res.heightAboveSeaLevel = 0;

    auto c = 0;
    for(auto iterator = lastSerialPackages.begin();
            iterator != lastSerialPackages.end(); iterator++, c++) {
        double weight = std::pow(0.5, (lastSerialPackages.size() - c));
        // Little workaround to get the sum of all c's to be 1
        if(c == 0) {
            weight *= 2;
        }

        res.heading = iterator->getChannel(0) * weight;
        res.roll = (iterator->getChannel(1) - 180) * weight;
        res.pitch = (iterator->getChannel(2) - 180);
        res.heightAboveGround = iterator->getChannel(3) * weight;
        res.heightAboveSeaLevel = iterator->getChannel(4) * weight;
    }

    res.flightmode = lastLoRaPackage.getChannel(0);
    res.armed = lastLoRaPackage.getChannel(1);

    lastState = res;

    return res;
}

Channel<rcLib::Package> &Fusion::getSerialIn() {
    return serialIn;
}

Channel<rcLib::Package> &Fusion::getLoRaIn() {
    return loraIn;
}

Channel<State_t> &Fusion::getChannelOut() {
    return out;
}

Channel<Gps_t> &Fusion::getGpsIn() {
    return gpsIn;
}
