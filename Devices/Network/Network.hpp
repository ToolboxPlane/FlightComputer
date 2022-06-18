/**
 * @file Network.hpp
 * @author paul
 * @date 18.03.19
 * @brief Network @TODO
 */

#ifndef FLIGHTCOMPUTER_NETWORK_HPP
#define FLIGHTCOMPUTER_NETWORK_HPP

#include <string>
#include <netinet/in.h>

#include "../../Framework/Node.hpp"
#include "../../Framework/InputChannel.hpp"
#include "../rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../../Framework/OutputChannel.hpp"

namespace device {
    class Network : public Node {
        public:
            explicit Network(const std::string &address, bool enable = true);

            auto getChannelIn() -> InputChannel<rcLib::Package> &;

        private:
            void run() override;

            int fd;
            sockaddr_in sockaddrIn{};
            InputChannel<rcLib::Package> in;

            static constexpr auto PROTOCOL_ID = 253;
    };
}

#endif //FLIGHTCOMPUTER_NETWORK_HPP
