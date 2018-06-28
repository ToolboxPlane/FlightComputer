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
            if(gpsIn.get(lastGpsMeasurement, false)) {
                gpsRecv = true;
            }
        }
        if (!pdbIn.isClosed()) {
            if(pdbIn.get(lastPdbPackage, false)) {
                pdbRecv = true;
            }
        }
        if(!taranisIn.isClosed()) {
            if(taranisIn.get(lastTaranisPackage, false)) {
                taranisRecv = true;
            }
        }
        if(!baseIn.isClosed()) {
            if(baseIn.get(lastBasePackage, false)) {
                baseRecv = true;
            }
        }
        if(!remoteIn.isClosed()) {
            if(remoteIn.get(lastRemotePackage, false)) {
                remoteRecv = true;
            }
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
    res.heightAboveGround = res.heightAboveSeaLevel; // Waiting for some kind of distance sensor
    res.airspeed = lastFcPackage.getChannel(5);
    res.accForward = lastFcPackage.getChannel(6);
    res.accSide = lastFcPackage.getChannel(7);
    res.accUpdown = lastFcPackage.getChannel(8);

    // Gps
    if(gpsRecv && lastGpsMeasurement.fixAquired) {
        res.position = lastGpsMeasurement;
        if (res.position.timestamp != lastState.position.timestamp) {
            res.groundSpeed = res.position.location.distanceTo(lastState.position.location) /
                              (res.position.timestamp - lastState.position.timestamp);
        } else {
            res.groundSpeed = lastState.groundSpeed;
        }
    } else {
        res.groundSpeed = res.airspeed;
        res.position.fixAquired = false;
    }

    // PDB
    if(pdbRecv) {
        res.voltage = (lastPdbPackage.getChannel(1) * 128) / 1000.0;
    } else {
        res.voltage = 16.8;
    }

    // Taranis
    if(taranisRecv) {
        res.taranis.throttle = normalizeTaranis(lastTaranisPackage.getChannel(5));
        res.taranis.yaw = normalizeTaranis(lastTaranisPackage.getChannel(6));
        res.taranis.pitch = normalizeTaranis(lastTaranisPackage.getChannel(7));
        res.taranis.roll= normalizeTaranis(lastTaranisPackage.getChannel(8));
        res.taranis.isArmed = normalizeTaranis(lastTaranisPackage.getChannel(9)) > 250;
        res.taranis.manualOverrideActive = normalizeTaranis(lastTaranisPackage.getChannel(10)) < 250;
    }

    if(remoteRecv) {
        res.lora.joyRight.x = (lastRemotePackage.getChannel(0)-127)/127.0;
        res.lora.joyRight.y = (lastRemotePackage.getChannel(1)-127)/127.0;
        res.lora.joyLeft.x = (lastRemotePackage.getChannel(2)-127)/127.0;
        res.lora.joyLeft.y = (lastRemotePackage.getChannel(3)-127)/127.0;
        res.lora.flightMode = static_cast<FlightMode>(lastRemotePackage.getChannel(4));
        res.lora.isArmed = static_cast<bool>(lastRemotePackage.getChannel(5));
    } else {
        res.lora.flightMode = FlightMode::HOLD;
    }

    //@TODO handle base, discuss which data to send

    lastState = res;

    return res;
}

int Fusion::normalizeTaranis(int input) {
    return (input-172)*1000/(1811-172);
}


#pragma clang diagnostic pop
