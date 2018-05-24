//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
#define FLIGHTCOMPUTER_SERIALSIMULATOR_HPP


#include "../../Channel.hpp"
#include "PackageExtended.hpp"
#include "../../ThreadModule.hpp"
#include "../../MultipleOutputChannel.hpp"

class RcLibSimulator : public ThreadModule{
public:
    explicit RcLibSimulator(uint8_t deviceId, const int intervalMs = 500);
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getChannelOut();

private:
    Channel<rcLib::PackageExtended> in;
    MultipleOutputChannel<rcLib::PackageExtended> out;
    const uint8_t deviceId;
    const int intervalMs;
};


#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
