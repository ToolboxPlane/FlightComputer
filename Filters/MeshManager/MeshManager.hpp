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

            InputChannel<rcLib::Package> &getPdbIn();

            OutputChannel<rcLib::Package> &getLoraOut();

            OutputChannel<rcLib::Package> &getSerialOut();


            InputChannel<rcLib::Package> &getFlightControllerIn();

            InputChannel<rcLib::Package> &getBaseIn();

            OutputChannel<rcLib::Package> &getFlightControllerOut();

            OutputChannel<rcLib::Package> &getRemoteOut();

            OutputChannel<rcLib::Package> &getBaseOut();

            OutputChannel<rcLib::Package> &getPdbOut();

            OutputChannel<rcLib::Package> &getTaranisOut();

            OutputChannel<rcLib::Package> &getNavOut();

        private:
            // Real output
            InputChannel<rcLib::Package> loraIn, serialIn, pdbIn;
            OutputChannel<rcLib::Package> loraOut, serialOut;

            // Internal
            InputChannel<rcLib::Package> flightControllerIn, baseIn;
            OutputChannel<rcLib::Package> flightControllerOut, remoteOut, baseOut, pdbOut, taranisOut, navOut;

            void propagateInternal(const rcLib::Package &pkg);

            void run() override;
    };
}

#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
