//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"

#define LAST_VALUES_LENGTH 64

void Fusion::run() {
    rcLib::Package lastSerialPackage;
    gps_t lastGps{};
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

state_t Fusion::process() {
    state_t res{};

    res.groundSpeed = 0;
    res.lat = lastGpsValues.back().lat;
    res.lon = lastGpsValues.back().lon;
    res.heightAboveSeaLevel = 0;

    res.airspeed = 0;

    res.heading = lastSerialPackages.back().getChannel(0);
    res.roll = lastSerialPackages.back().getChannel(1) - 180;
    res.pitch = lastSerialPackages.back().getChannel(2) - 180;
    res.heightAboveGround = lastSerialPackages.back().getChannel(3);
    res.heightAboveSeaLevel = lastSerialPackages.back().getChannel(4);
    if(res.heightAboveGround > 2.5) {
        // Do stuff using SRTM data
    }

    res.flightmode = lastLoRaPackage.getChannel(0);
    res.armed = lastLoRaPackage.getChannel(1);

    return res;
}

Channel<rcLib::Package> &Fusion::getSerialIn() {
    return serialIn;
}

Channel<rcLib::Package> &Fusion::getLoRaIn() {
    return loraIn;
}

Channel<state_t> &Fusion::getChannelOut() {
    return out;
}

Channel<gps_t> &Fusion::getGpsIn() {
    return gpsIn;
}
