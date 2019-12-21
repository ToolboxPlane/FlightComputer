//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../Node.hpp"
#include "../../Devices/rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../../InputChannel.hpp"
#include "../../OutputChannel.hpp"
namespace filter {
    class MeshManager : public Node {
    public:
        MeshManager();

        InputChannel<rcLib::Package> &getLoraIn();

        InputChannel<rcLib::Package> &getSerialIn();

        OutputChannel<rcLib::Package> &getLoraOut();

        OutputChannel<rcLib::Package> &getSerialOut();

        InputChannel<rcLib::Package> &getFlightControllerIn();

        InputChannel<rcLib::Package> &getBaseIn();

        InputChannel<rcLib::Package> &getRemoteIn();

        OutputChannel<rcLib::Package> &getFlightControllerOut();

        OutputChannel<rcLib::Package> &getRemoteOut();

        OutputChannel<rcLib::Package> &getBaseOut();

        OutputChannel<rcLib::Package> &getPdbOut();

        OutputChannel<rcLib::Package> &getTaranisOut();

    private:
        InputChannel<rcLib::Package> loraIn, serialIn;
        InputChannel<rcLib::Package> flightControllerIn, remoteIn, baseIn;
        OutputChannel<rcLib::Package> loraOut, serialOut;
        OutputChannel<rcLib::Package> flightControllerOut, remoteOut, baseOut, pdbOut, taranisOut;

        void propagateInternal(rcLib::Package pkg);

        void run() override;
    };
}

#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
