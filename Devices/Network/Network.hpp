/**
 * @file Network.hpp
 * @author paul
 * @date 18.03.19
 * @brief Network @TODO
 */

#ifndef FLIGHTCOMPUTER_NETWORK_HPP
#define FLIGHTCOMPUTER_NETWORK_HPP

#include <netinet/in.h>
#include <string>

#include "../../Framework/InputChannel.hpp"
#include "../../Framework/Node.hpp"
#include "../../Framework/OutputChannel.hpp"
#include "../rcLib/RadioControlProtocolCpp/rcLib.hpp"

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
} // namespace device

#endif // FLIGHTCOMPUTER_NETWORK_HPP
