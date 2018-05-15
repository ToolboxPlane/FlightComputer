//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../ThreadModule.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../Channel.hpp"


class MeshManager : ThreadModule{
public:
    MeshManager();

    Channel<rcLib::PackageExtended> &getLoraIn();
    Channel<rcLib::PackageExtended> &getSerialIn();
    Channel<rcLib::PackageExtended> &getLoraOut();
    Channel<rcLib::PackageExtended> &getSerialOut();

    Channel<rcLib::PackageExtended> &getFlightControllerIn() {
        return flightControllerIn;
    }

    Channel<rcLib::PackageExtended> &getFlightControllerOut() {
        return flightControllerOut;
    }
    Channel<rcLib::PackageExtended> &getRemoteIn() {
        return remoteIn;
    }

    Channel<rcLib::PackageExtended> &getRemoteOut() {
        return remoteOut;
    }

    Channel<rcLib::PackageExtended> &getBaseIn() {
        return baseIn;
    }

    Channel<rcLib::PackageExtended> &getBaseOut() {
        return baseOut;
    }

    Channel<rcLib::PackageExtended> &getPdbOut() {
        return pdbOut;
    }
private:
    Channel<rcLib::PackageExtended> loraIn, serialIn, loraOut, serialOut, pdbOut;
    Channel<rcLib::PackageExtended> flightControllerOut, remoteOut, baseOut, flightControllerIn, remoteIn, baseIn;

    void propagateInternal(rcLib::PackageExtended pkg);

    void run() override;
};


#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
