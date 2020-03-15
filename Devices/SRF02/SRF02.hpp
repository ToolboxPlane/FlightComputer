/**
 * @file SRF02.hpp
 * @author paul
 * @date 14.03.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_SRF02_HPP
#define FLIGHTCOMPUTER_SRF02_HPP

#include "../Serial/SerialPosix.hpp"
#include "../../Utilities/Si/SiLiterals.hpp"

namespace device {
    class SRF02 : public Node {
        public:
            explicit SRF02(const std::string &port);

            auto getChannelOut() -> OutputChannel<si::base::Meter<>>;

        private:
            void run() override;

            void sendBuff(const std::vector<uint8_t> &buf);

            OutputChannel<si::base::Meter<>> out;
            int fd;
    };
}

#endif //FLIGHTCOMPUTER_SRF02_HPP
