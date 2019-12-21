/**
 * @file Network.hpp
 * @author paul
 * @date 18.03.19
 * @brief Network @TODO
 */

#ifndef FLIGHTCOMPUTER_NETWORK_HPP
#define FLIGHTCOMPUTER_NETWORK_HPP


#include "../../Node.hpp"
#include "../../InputChannel.hpp"
#include "../rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../../OutputChannel.hpp"

namespace device {
    class Network : public Node {
    public:
        explicit Network(std::string address);

        auto getChannelIn() -> InputChannel<rcLib::Package>&;
        auto getChannelOut() -> OutputChannel<rcLib::Package>&;
    private:
        void run() override;

        std::string address;
        int fd;
        InputChannel<rcLib::Package> in;
        OutputChannel<rcLib::Package> out;

        static constexpr auto PROTOCOL_ID = 253;
    };
}

#endif //FLIGHTCOMPUTER_NETWORK_HPP
