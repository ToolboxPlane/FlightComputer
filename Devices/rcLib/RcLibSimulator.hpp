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
    explicit RcLibSimultator(const uint8_t deviceId) : deviceId(deviceId), ThreadModule() {};
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    Channel<rcLib::PackageExtended> &getChannelOut();

private:
    Channel<rcLib::PackageExtended> in, out;
    const uint8_t deviceId;
};


#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
