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
private:
    Channel<rcLib::PackageExtended> loraIn, serialIn, loraOut, serialOut;

    void run() override;
};


#endif //FLIGHTCOMPUTER_MESHMANAGER_HPP
