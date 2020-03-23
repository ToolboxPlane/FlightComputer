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

    InputChannel<rcLib::Package> &MeshManager::getLoraIn() {
        return loraIn;
    }

    InputChannel<rcLib::Package> &MeshManager::getSerialIn() {
        return serialIn;
    }

    OutputChannel<rcLib::Package> &MeshManager::getLoraOut() {
        return loraOut;
    }

    OutputChannel<rcLib::Package> &MeshManager::getSerialOut() {
        return serialOut;
    }

    InputChannel<rcLib::Package> &MeshManager::getFlightControllerIn() {
        return flightControllerIn;
    }

    InputChannel<rcLib::Package> &MeshManager::getBaseIn() {
        return baseIn;
    }

    InputChannel<rcLib::Package> &MeshManager::getRemoteIn() {
        return remoteIn;
    }

    OutputChannel<rcLib::Package> &MeshManager::getFlightControllerOut() {
        return flightControllerOut;
    }

    OutputChannel<rcLib::Package> &MeshManager::getRemoteOut() {
        return remoteOut;
    }

    OutputChannel<rcLib::Package> &MeshManager::getBaseOut() {
        return baseOut;
    }

    OutputChannel<rcLib::Package> &MeshManager::getPdbOut() {
        return pdbOut;
    }

    OutputChannel<rcLib::Package> &MeshManager::getTaranisOut() {
        return taranisOut;
    }

    void MeshManager::run() {
        rcLib::Package pkg{};
        while (!serialIn.isClosed()) {
            if (loraIn.get(pkg, false)) {
                if (pkg.needsForwarding()) {
                    pkg.countNode();
                }
                propagateInternal(pkg);
            }
            if (serialIn.get(pkg, false)) {
                if (pkg.needsForwarding()) {
                    pkg.countNode();
                }
                propagateInternal(pkg);
            }
            if (pdbIn.get(pkg, false))  {
                if (pkg.needsForwarding()) {
                    pkg.countNode();
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


    void MeshManager::propagateInternal(rcLib::Package pkg) {
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
                std::cerr << "[MeshManager]: Some external device creates loops in the network\n";
                break;
        }
    }

    InputChannel<rcLib::Package> &MeshManager::getPdbIn() {
        return pdbIn;
    }
}
