//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "MeshManager.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
enum class RCLIB_DEVICE_ID {
    REMOTE = 17, FLIGHT_COMPUTER = 38,
    FLIGHT_CONTROLLER = 23,
    BASE = 63, POWER_DISTRIBUTION = 74, TARANIS = 56
};

MeshManager::MeshManager() {
    this->start();
}

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

Channel<rcLib::PackageExtended> &MeshManager::getFlightControllerIn() {
    return flightControllerIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getBaseIn() {
    return baseIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getRemoteIn() {
    return remoteIn;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getFlightControllerOut() {
    return flightControllerOut;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getRemoteOut() {
    return remoteOut;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getBaseOut() {
    return baseOut;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getPdbOut() {
    return pdbOut;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getTaranisOut() {
    return taranisOut;
}

Channel<rcLib::PackageExtended> &MeshManager::getTcpIn() {
    return tcpIn;
}

MultipleOutputChannel<rcLib::PackageExtended> &MeshManager::getTcpOut() {
    return tcpOut;
}

void MeshManager::run() {
    rcLib::PackageExtended pkg;
    while(true) {
        if (loraIn.get(pkg, false)) {
            if(pkg.needsForwarding()) {
                pkg.countNode();
                serialOut.put(pkg);
            }
            tcpOut.put(pkg);
            propagateInternal(pkg);
        }
        if (serialIn.get(pkg, false)) {
            if(pkg.needsForwarding())  {
                pkg.countNode();
                loraOut.put(pkg);
            }
            tcpOut.put(pkg);
            propagateInternal(pkg);
        }
        if (tcpIn.get(pkg, false)) {
            if(pkg.needsForwarding())  {
                pkg.countNode();
                loraOut.put(pkg);
                serialOut.put(pkg);
            }
            propagateInternal(pkg);
        }
        if (flightControllerIn.get(pkg, false)) {
            pkg.setMeshProperties(static_cast<uint8_t>(false));
            pkg.setDeviceId(static_cast<uint8_t>(RCLIB_DEVICE_ID::FLIGHT_COMPUTER));
            serialOut.put(pkg);
            tcpOut.put(pkg);
        }
        if (remoteIn.get(pkg, false)) {
            pkg.setMeshProperties(static_cast<uint8_t>(false));
            pkg.setDeviceId(static_cast<uint8_t>(RCLIB_DEVICE_ID::FLIGHT_COMPUTER));
            loraOut.put(pkg);
        }
        if (baseIn.get(pkg, false)) {
            pkg.setMeshProperties(static_cast<uint8_t>(true), 2);
            pkg.setDeviceId(static_cast<uint8_t>(RCLIB_DEVICE_ID::FLIGHT_COMPUTER));
            loraOut.put(pkg);
        }
        std::this_thread::yield();
    }
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
        case RCLIB_DEVICE_ID::FLIGHT_COMPUTER:
            // We routed in a loop...
            break;
    }
}


#pragma clang diagnostic pop