//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../ThreadModule.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../Channel.hpp"
#include "../../MultipleOutputChannel.hpp"

class MeshManager : ThreadModule{
public:
    MeshManager();

    Channel<rcLib::PackageExtended> &getLoraIn();
    Channel<rcLib::PackageExtended> &getSerialIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getLoraOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getSerialOut();

    Channel<rcLib::PackageExtended> &getFlightControllerIn() {
        return flightControllerIn;
    }

    MultipleOutputChannel<rcLib::PackageExtended> &getFlightControllerOut() {
        return flightControllerOut;
    }
    Channel<rcLib::PackageExtended> &getRemoteIn() {
        return remoteIn;
    }

    MultipleOutputChannel<rcLib::PackageExtended> &getRemoteOut() {
        return remoteOut;
    }

    Channel<rcLib::PackageExtended> &getBaseIn() {
        return baseIn;
    }

    MultipleOutputChannel<rcLib::PackageExtended> &getBaseOut() {
        return baseOut;
    }

    MultipleOutputChannel<rcLib::PackageExtended> &getPdbOut() {
        return pdbOut;
    }

    MultipleOutputChannel<rcLib::PackageExtended> &getTaranisOut() {
        return taranisOut;
    }
private:
    Channel<rcLib::PackageExtended> loraIn, serialIn;
    Channel<rcLib::PackageExtended> flightControllerIn, remoteIn, baseIn;
    MultipleOutputChannel<rcLib::PackageExtended> loraOut, serialOut;
    MultipleOutputChannel<rcLib::PackageExtended> flightControllerOut, remoteOut, baseOut, pdbOut, taranisOut;

    void propagateInternal(rcLib::PackageExtended pkg);

    void run() override;
};


#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
