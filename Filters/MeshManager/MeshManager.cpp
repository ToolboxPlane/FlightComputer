//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "../../Devices/rcLib/PackageOstream.hpp"
#include "MeshManager.hpp"

namespace filter {
    enum class DeviceId {
        REMOTE = 17,
        FLIGHT_CONTROLLER = 23,
        FLIGHT_COMPUTER = 38,
        TARANIS = 56,
        BASE = 63,
        POWER_DISTRIBUTION = 74,
        NAV_BOARD = 91
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

    InputChannel<rcLib::Package> &MeshManager::getPdbIn() {
        return pdbIn;
    }

    OutputChannel<rcLib::Package> &MeshManager::getTaranisOut() {
        return taranisOut;
    }

    OutputChannel<rcLib::Package> &MeshManager::getNavOut() {
        return navOut;
    }

    void MeshManager::run() {
        rcLib::Package pkg{};
        while (!serialIn.isClosed()) {
            if (loraIn.get(pkg, false)) {
                propagateInternal(pkg);
            }
            if (serialIn.get(pkg, false)) {
                propagateInternal(pkg);
            }
            if (pdbIn.get(pkg, false)) {
                propagateInternal(pkg);
            }

            if (flightControllerIn.get(pkg, false)) {
                serialOut.put(pkg);
            }
            if (baseIn.get(pkg, false)) {
                loraOut.put(pkg);
            }
            std::this_thread::yield();
        }
    }


    void MeshManager::propagateInternal(const rcLib::Package &pkg) {
        switch (static_cast<DeviceId>(pkg.getDeviceId())) {
            case DeviceId::REMOTE:
                remoteOut.put(pkg);
                break;
            case DeviceId::FLIGHT_CONTROLLER:
                flightControllerOut.put(pkg);
                break;
            case DeviceId::BASE:
                baseOut.put(pkg);
                break;
            case DeviceId::POWER_DISTRIBUTION:
                pdbOut.put(pkg);
                break;
            case DeviceId::TARANIS:
                taranisOut.put(pkg);
                break;
            case DeviceId::NAV_BOARD:
                navOut.put(pkg);
                break;
            case DeviceId::FLIGHT_COMPUTER:
                // We routed in a loop...
                std::cerr << "[MeshManager]:\tSome external device creates loops in the network: " << pkg << std::endl;
                break;
            default:
                std::cerr << "[MeshManager]:\tUnknown Device" << std::endl;
                break;
        }
    }

}
