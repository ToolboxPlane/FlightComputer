//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../Node.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../InputChannel.hpp"
#include "../../OutputChannel.hpp"
namespace filter {
    class MeshManager : public Node {
    public:
        MeshManager();

        InputChannel<rcLib::PackageExtended> &getLoraIn();

        InputChannel<rcLib::PackageExtended> &getSerialIn();

        OutputChannel<rcLib::PackageExtended> &getLoraOut();

        OutputChannel<rcLib::PackageExtended> &getSerialOut();

        InputChannel<rcLib::PackageExtended> &getFlightControllerIn();

        InputChannel<rcLib::PackageExtended> &getBaseIn();

        InputChannel<rcLib::PackageExtended> &getRemoteIn();

        OutputChannel<rcLib::PackageExtended> &getFlightControllerOut();

        OutputChannel<rcLib::PackageExtended> &getRemoteOut();

        OutputChannel<rcLib::PackageExtended> &getBaseOut();

        OutputChannel<rcLib::PackageExtended> &getPdbOut();

        OutputChannel<rcLib::PackageExtended> &getTaranisOut();

    private:
        InputChannel<rcLib::PackageExtended> loraIn, serialIn;
        InputChannel<rcLib::PackageExtended> flightControllerIn, remoteIn, baseIn;
        OutputChannel<rcLib::PackageExtended> loraOut, serialOut;
        OutputChannel<rcLib::PackageExtended> flightControllerOut, remoteOut, baseOut, pdbOut, taranisOut;

        void propagateInternal(rcLib::PackageExtended pkg);

        void run() override;
    };
}

#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
