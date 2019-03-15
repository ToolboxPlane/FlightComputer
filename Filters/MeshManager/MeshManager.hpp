//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../Filter.hpp"
#include "../../Devices/rcLib/PackageExtended.hpp"
#include "../../InputChannel.hpp"
#include "../../OutputChannel.hpp"

class MeshManager : public Filter{
public:
    MeshManager();

    InputChannel<rcLib::PackageExtended> &getLoraIn();
    InputChannel<rcLib::PackageExtended> &getSerialIn();
    OutputChannel<rcLib::PackageExtended> &getLoraOut();
    OutputChannel<rcLib::PackageExtended> &getSerialOut();

    InputChannel<rcLib::PackageExtended> &getFlightControllerIn();
    InputChannel<rcLib::PackageExtended> &getBaseIn();
    InputChannel<rcLib::PackageExtended> &getRemoteIn();
    InputChannel<rcLib::PackageExtended> &getTcpIn();

    OutputChannel<rcLib::PackageExtended> &getFlightControllerOut();
    OutputChannel<rcLib::PackageExtended> &getRemoteOut();
    OutputChannel<rcLib::PackageExtended> &getBaseOut();
    OutputChannel<rcLib::PackageExtended> &getPdbOut();
    OutputChannel<rcLib::PackageExtended> &getTaranisOut();
    OutputChannel<rcLib::PackageExtended> &getTcpOut();
private:
    InputChannel<rcLib::PackageExtended> loraIn, serialIn, tcpIn;
    InputChannel<rcLib::PackageExtended> flightControllerIn, remoteIn, baseIn;
    OutputChannel<rcLib::PackageExtended> loraOut, serialOut, tcpOut;
    OutputChannel<rcLib::PackageExtended> flightControllerOut, remoteOut, baseOut, pdbOut, taranisOut;

    void propagateInternal(rcLib::PackageExtended pkg);

    void run() override;
};


#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
