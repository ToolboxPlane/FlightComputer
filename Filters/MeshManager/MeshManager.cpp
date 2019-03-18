//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "MeshManager.hpp"

namespace filter {
    enum class RCLIB_DEVICE_ID {
        REMOTE = 17, FLIGHT_COMPUTER = 38,
        FLIGHT_CONTROLLER = 23,
        BASE = 63, POWER_DISTRIBUTION = 74, TARANIS = 56
    };

    MeshManager::MeshManager() {
        this->start();
    }

    InputChannel<rcLib::PackageExtended> &MeshManager::getLoraIn() {
        return loraIn;
    }

    InputChannel<rcLib::PackageExtended> &MeshManager::getSerialIn() {
        return serialIn;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getLoraOut() {
        return loraOut;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getSerialOut() {
        return serialOut;
    }

    InputChannel<rcLib::PackageExtended> &MeshManager::getFlightControllerIn() {
        return flightControllerIn;
    }

    InputChannel<rcLib::PackageExtended> &MeshManager::getBaseIn() {
        return baseIn;
    }

    InputChannel<rcLib::PackageExtended> &MeshManager::getRemoteIn() {
        return remoteIn;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getFlightControllerOut() {
        return flightControllerOut;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getRemoteOut() {
        return remoteOut;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getBaseOut() {
        return baseOut;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getPdbOut() {
        return pdbOut;
    }

    OutputChannel<rcLib::PackageExtended> &MeshManager::getTaranisOut() {
        return taranisOut;
    }

    void MeshManager::run() {
        rcLib::PackageExtended pkg{};
        while (!serialIn.isClosed()) {
            if (loraIn.get(pkg, false)) {
                if (pkg.needsForwarding()) {
                    pkg.countNode();
                    serialOut.put(pkg);
                }
                propagateInternal(pkg);
            }
            if (serialIn.get(pkg, false)) {
                if (pkg.needsForwarding()) {
                    pkg.countNode();
                    loraOut.put(pkg);
                }
                propagateInternal(pkg);
            }
            if (flightControllerIn.get(pkg, false)) {
                pkg.setMeshProperties(static_cast<uint8_t>(false));
                pkg.setDeviceId(static_cast<uint8_t>(RCLIB_DEVICE_ID::FLIGHT_COMPUTER));
                serialOut.put(pkg);
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
        switch ((RCLIB_DEVICE_ID) pkg.getDeviceId()) {
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
                std::cerr << "[MeshManager]: Some external device creates loops in the network";
                break;
        }
    }
}