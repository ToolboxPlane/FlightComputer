//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"

void Fusion::run() {
    rcLib::Package lastSerialPackage, lastLoRaPackage;
    ProcessingStatus serialStatus = NOT_RECEIVED, loraStatus = NOT_RECEIVED;
    while(!out.isClosed()) {
        if(!loraIn.isClosed() && loraIn.get(lastLoRaPackage, false)) {
            loraStatus = READY;
        } else if( !serialIn.isClosed() && serialIn.get(lastSerialPackage, false)) {
            serialStatus = READY;
        }

        if(loraStatus == READY || serialStatus == READY) {
            if(loraStatus != NOT_RECEIVED && serialStatus != NOT_RECEIVED) {
                out.put(process(lastLoRaPackage, lastSerialPackage));
                loraStatus = PROCESSED;
                serialStatus = PROCESSED;
            } else {
                // Maybe do other stuff
            }
        } else {
            std::this_thread::yield();
        }
    }
}

state_t Fusion::process(rcLib::Package loraPackage, rcLib::Package serialPackage) {
    state_t res{};

    res.groundSpeed = 0;
    res.lat = res.lon = 0;
    res.heightAboveSeaLevel = 0;

    res.airspeed = 0;

    res.heading = serialPackage.getChannel(0);
    res.roll = serialPackage.getChannel(1) - 180;
    res.pitch = serialPackage.getChannel(2) - 180;
    res.heightAboveGround = serialPackage.getChannel(3);
    res.heightAboveSeaLevel = serialPackage.getChannel(4);
    if(res.heightAboveGround > 2.5) {
        // Do stuff using SRTM data
    }

    res.flightmode = loraPackage.getChannel(0);
    res.armed = loraPackage.getChannel(1);

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
