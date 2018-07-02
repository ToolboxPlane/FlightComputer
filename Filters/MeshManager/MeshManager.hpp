//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../Filter.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../Channel.hpp"
#include "../../MultipleOutputChannel.hpp"

class MeshManager : Filter{
public:
    MeshManager();

    Channel<rcLib::PackageExtended> &getLoraIn();
    Channel<rcLib::PackageExtended> &getSerialIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getLoraOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getSerialOut();

    Channel<rcLib::PackageExtended> &getFlightControllerIn();
    Channel<rcLib::PackageExtended> &getBaseIn();
    Channel<rcLib::PackageExtended> &getRemoteIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getFlightControllerOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getRemoteOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getBaseOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getPdbOut();
    MultipleOutputChannel<rcLib::PackageExtended> &getTaranisOut();

private:
    Channel<rcLib::PackageExtended> loraIn, serialIn;
    Channel<rcLib::PackageExtended> flightControllerIn, remoteIn, baseIn;
    MultipleOutputChannel<rcLib::PackageExtended> loraOut, serialOut;
    MultipleOutputChannel<rcLib::PackageExtended> flightControllerOut, remoteOut, baseOut, pdbOut, taranisOut;

    void propagateInternal(rcLib::PackageExtended pkg);

    void run() override;
};


#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
