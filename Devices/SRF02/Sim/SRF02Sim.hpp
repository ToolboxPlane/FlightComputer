/**
 * @file SRF02Sim.hpp
 * @author paul
 * @date 26.03.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_SRF02SIM_HPP
#define FLIGHTCOMPUTER_SRF02SIM_HPP

#include <SiBase.hpp>

#include "../../../OutputChannel.hpp"
#include "../../../Node.hpp"

namespace device {
    class SRF02Sim : public Node {
        public:
            SRF02Sim();
            auto getChannelOut() -> OutputChannel<si::Meter<>> &;

        private:
            void run() override;

            OutputChannel <si::Meter<>> out;
    };

}

#endif //FLIGHTCOMPUTER_SRF02SIM_HPP
