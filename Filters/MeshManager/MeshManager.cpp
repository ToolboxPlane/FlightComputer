//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "MeshManager.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
enum class RCLIB_DEVICE_ID {
    REMOTE = 17,
    FLIGHT_CONTROLLER = 23,
    BASE = 63, POWER_DISTRIBUTION = 74, TARANIS = 56
};

Channel<rcLib::PackageExtended> &MeshManager::getLoraIn() {
    return loraIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getSerialIn() {
    return serialIn;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getLoraOut() {
    return loraOut;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getSerialOut() {
    return serialOut;
}

void MeshManager::run() {
    rcLib::PackageExtended pkg;
    while(true) {
        if (loraIn.get(pkg, false)) {
            if(pkg.needsForwarding()) {
                pkg.countNode();
                serialOut.put(pkg);
            }
            propagateInternal(pkg);
        }
        if (serialIn.get(pkg, false)) {
            if(pkg.needsForwarding())  {
                pkg.countNode();
                loraOut.put(pkg);
            }
            propagateInternal(pkg);
        }
        if (flightControllerIn.get(pkg, false)) {
            pkg.setMeshProperties(false);
            serialOut.put(pkg);
        }
        if (remoteIn.get(pkg, false)) {
            pkg.setMeshProperties(false);
            loraOut.put(pkg);
        }
        if (baseIn.get(pkg, false)) {
            pkg.setMeshProperties(true, 2);
            loraOut.put(pkg);
        }
        std::this_thread::yield();
    }
}

MeshManager::MeshManager() {
    this->start();
}

void MeshManager::propagateInternal(rcLib::PackageExtended pkg) {
    switch ((RCLIB_DEVICE_ID)pkg.getDeviceId()) {
        case RCLIB_DEVICE_ID::REMOTE:
            remoteOut.put(pkg);
            break;
        case RCLIB_DEVICE_ID::FLIGHT_CONTROLLER:
            flightControllerOut.put(pkg);
            break;
        case RCLIB_DEVICE_ID::BASE:
            baseOut.put(pkg);
            break;
        case RCLIB_DEVICE_ID::POWER_DISTRIBUTION:
            pdbOut.put(pkg);
            break;
        case RCLIB_DEVICE_ID::TARANIS:
            taranisOut.put(pkg);
            break;
    }
}

#pragma clang diagnostic pop