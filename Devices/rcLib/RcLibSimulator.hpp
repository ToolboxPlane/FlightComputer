//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
#define FLIGHTCOMPUTER_SERIALSIMULATOR_HPP


#include "../../InputChannel.hpp"
#include "RadioControlProtocolCpp/rcLib.hpp"
#include "../../Node.hpp"
#include "../../OutputChannel.hpp"

namespace device {
    class RcLibSimulator : public Node {
    public:
        explicit RcLibSimulator(uint8_t deviceId, int intervalMs = 500);

        void run() override;

        auto getChannelIn() -> InputChannel<rcLib::Package>&;

        auto getChannelOut() -> OutputChannel<rcLib::Package>&;

    private:
        InputChannel<rcLib::Package> in;
        OutputChannel<rcLib::Package> out;
        const uint8_t deviceId;
        const int intervalMs;
    };
}

#endif //FLIGHTCOMPUTER_SERIALSIMULATOR_HPP
