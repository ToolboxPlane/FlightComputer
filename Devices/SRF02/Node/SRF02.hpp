/**
 * @file SRF02.hpp
 * @author paul
 * @date 14.03.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_SRF02_HPP
#define FLIGHTCOMPUTER_SRF02_HPP

#include <SiBase.hpp>
#include <vector>

#include "../../../Framework/Node.hpp"
#include "../../../Framework/OutputChannel.hpp"

namespace device {
    class SRF02 : public Node {
      public:
        explicit SRF02(const std::string &port);

        auto getChannelOut() -> OutputChannel<si::Meter<>> &;

      private:
        void run() override;

        void sendBuff(const std::vector<uint8_t> &buf);

        OutputChannel<si::Meter<>> out;
        int fd;
    };
} // namespace device

#endif // FLIGHTCOMPUTER_SRF02_HPP
