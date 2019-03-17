//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
#define FLIGHTCOMPUTER_SERIALSIMULATOR_HPP


#include "../../InputChannel.hpp"
#include "PackageExtended.hpp"
#include "../../Node.hpp"
#include "../../OutputChannel.hpp"

namespace device {
    class RcLibSimulator : public Node {
    public:
        explicit RcLibSimulator(uint8_t deviceId, int intervalMs = 500);

        void run() override;

        InputChannel<rcLib::PackageExtended> &getChannelIn();

        OutputChannel<rcLib::PackageExtended> &getChannelOut();

    private:
        InputChannel<rcLib::PackageExtended> in;
        OutputChannel<rcLib::PackageExtended> out;
        const uint8_t deviceId;
        const int intervalMs;
    };
}

#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
