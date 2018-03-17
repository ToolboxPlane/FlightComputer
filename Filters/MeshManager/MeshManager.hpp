//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_MESHMANAGER_HPP
#define FLIGHTCOMPUTER_MESHMANAGER_HPP


#include "../../ThreadModule.hpp"
#include "../../Devices/RadioControlProtocol/rcLib.hpp"
#include "../../Channel.hpp"

class MeshManager : ThreadModule{
public:
    MeshManager();

    Channel<rcLib::Package> &getLoraIn();
    Channel<rcLib::Package> &getSerialIn();
    Channel<rcLib::Package> &getLoraOut();
    Channel<rcLib::Package> &getSerialOut();
private:
    Channel<rcLib::Package> loraIn, serialIn, loraOut, serialOut;

    void run() override;
};


#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
