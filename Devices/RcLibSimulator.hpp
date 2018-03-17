//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
#define FLIGHTCOMPUTER_SERIALSIMULATOR_HPP


#include "../Channel.hpp"
#include "RadioControlProtocol/rcLib.hpp"
#include "../ThreadModule.hpp"

class RcLibSimultator : public ThreadModule{
public:
    RcLibSimultator() : ThreadModule() {};
    void run() override;

    Channel<rcLib::Package> &getChannelIn();
    Channel<rcLib::Package> &getChannelOut();

private:
    Channel<rcLib::Package> in, out;
};


#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
