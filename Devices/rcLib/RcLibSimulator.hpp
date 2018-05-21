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
    explicit RcLibSimulator(const uint8_t deviceId);
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getChannelOut();

private:
    Channel<rcLib::PackageExtended> in;
    MultipleOutputChannel<rcLib::PackageExtended> out;
    const uint8_t deviceId;
};


#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
