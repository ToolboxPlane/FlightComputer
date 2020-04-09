/**
 * @file SerialInterface.hpp
 * @author paul
 * @date 29.12.18
 * @brief Definition of an interface definition conforming to POSIX
 * @see this class is adapted from the serial toolbox: github.com/aul12/SerialToolbox
 */

#ifndef SERIALTOOLBOX_INTERFACEPOSIX_HPP
#define SERIALTOOLBOX_INTERFACEPOSIX_HPP

#include <string>
#include "../../Node.hpp"
#include "../../InputChannel.hpp"
#include "../rcLib/RadioControlProtocolCpp/rcLib.hpp"
#include "../../OutputChannel.hpp"

namespace device {
    enum class Parity {
        NONE = 0, EVEN = 1, ODD = 2, SPACE = 3, MARK = 4
    };

    /**
     * Implements an Interface for Posix conforming systems (POSIX.1-2001). Tested
     * on Linux Kernel 4.15.
     */
    class SerialPosix : public Node {
        public:
            explicit SerialPosix(const std::string &port, int baud = 9600);

            void setBaud(int baud);

            void setPort(const std::string &port);

            void setParity(Parity parity);

            void setDataBits(int dataBits);

            void setStopBits(int stopBits);

            auto getChannelIn() -> InputChannel<rcLib::Package> &;

            auto getChannelOut() -> OutputChannel<rcLib::Package> &;

            ~SerialPosix();

        private:
            void run() override;

            void sendBuff(const uint8_t *buffer, std::size_t len) const;

            int fd;

            InputChannel<rcLib::Package> in;
            OutputChannel<rcLib::Package> out;

            static constexpr std::size_t BUF_SIZE = 512;
    };
}

#endif //HTERMCLONE_SERIALINTERFACE_HPP
