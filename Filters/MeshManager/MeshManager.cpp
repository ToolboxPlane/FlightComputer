//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "MeshManager.hpp"

enum class RCLIB_DEVICE_ID {
    REMOTE = 17,
    FLIGHT_CONTROLLER = 23,
    BASE = 63, POWER_DISTRIBUTION = 74
};

Channel<rcLib::PackageExtended> &MeshManager::getLoraIn() {
    return loraIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getSerialIn() {
    return serialIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getLoraOut() {
    return loraOut;
}

Channel<rcLib::PackageExtended> &MeshManager::getSerialOut() {
    return serialOut;
}

void MeshManager::run() {
    rcLib::PackageExtended pkg;
    while(!loraIn.isClosed() || !serialIn.isClosed() || !loraOut.isClosed() || !serialOut.isClosed()) {
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
    }
}

MeshManager::MeshManager() : ThreadModule(){
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
    }
}
