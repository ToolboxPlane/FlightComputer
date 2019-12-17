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
#include "../rcLib/PackageExtended.hpp"
#include "../../OutputChannel.hpp"

namespace device {
    class Network : public Node {
    public:
        explicit Network(std::string address);

        auto getChannelIn() -> InputChannel<rcLib::PackageExtended>&;
        auto getChannelOut() -> OutputChannel<rcLib::PackageExtended>&;
    private:
        void run() override;

        std::string address;
        int fd;
        InputChannel<rcLib::PackageExtended> in;
        OutputChannel<rcLib::PackageExtended> out;

        static constexpr auto PROTOCOL_ID = 253;
    };
}

#endif //FLIGHTCOMPUTER_NETWORK_HPP
