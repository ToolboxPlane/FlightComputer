//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
#define FLIGHTCOMPUTER_SERIALSIMULATOR_HPP


#include "../../Channel.hpp"
#include "PackageExtended.hpp"
#include "../../ThreadModule.hpp"

class RcLibSimultator : public ThreadModule{
public:
    RcLibSimultator() : ThreadModule() {};
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    Channel<rcLib::PackageExtended> &getChannelOut();

private:
    Channel<rcLib::PackageExtended> in, out;
};


#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
