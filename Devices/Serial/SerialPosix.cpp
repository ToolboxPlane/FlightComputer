/**
 * @file SerialInterface.cpp
 * @author paul
 * @date 29.12.18
 * @brief Implementation of an interface for posix compliant systems
 */

#include "SerialPosix.hpp"

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <mutex>
#include <regex>
#include <termios.h>
#include <unistd.h>
#include <vector>

namespace device {

    SerialPosix::SerialPosix(const std::string &port, int baud) : fd{-1} {
        this->setPort(port);
        this->setBaud(baud);
        this->setParity(Parity::NONE);
        this->setDataBits(8);
        this->setStopBits(1);

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }

        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;                 // disable break processing
        tty.c_lflag = 0;                        // no signaling chars, no echo, no canonical processing
        tty.c_oflag = 0;                        // no remapping, no delays
        tty.c_cc[VMIN] = 0;                     // polling read
        tty.c_cc[VTIME] = 0;                    // read is non blocking
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
        tty.c_cflag |= (CLOCAL | CREAD);        // ignore modem controls, enable reading

#ifdef CRTSCTS
        tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control if possible (not Posix, only Linux)
#endif

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }

        this->start();
    }

    void SerialPosix::run() {
        std::array<uint8_t, BUF_SIZE> buffer{};
        rcLib::Package pkgIn, pkgOut;

        while (!in.isClosed()) {
            /*
             * Go fix your language (en). "read" needs to get a decent past tense form,
             * i decided on "readed" instead of the phonetically incorrect and irregular
             * "read" (https://www.youtube.com/watch?v=A8zWWp0akUU).
             */
            auto readed = read(this->fd, buffer.data(), BUF_SIZE);
            for (auto c = 0; c < readed; c++) {
                if (pkgOut.decode(buffer[c])) {
                    out.put(pkgOut);
                }
            }

            while (in.get(pkgIn, false)) {
                auto len = pkgIn.encode();
                this->sendBuff(pkgIn.getEncodedData(), len);
            }
            std::this_thread::yield();
        }
    }

    void SerialPosix::sendBuff(const uint8_t *buffer, std::size_t len) const {
        std::size_t written = 0;
        do {
            auto result = write(this->fd, buffer + written, len - written);
            if (result < 0) {
                std::cerr << "SerialPosix:\t" << strerror(errno) << std::endl;
            } else {
                written += static_cast<std::size_t>(result);
            }
        } while (written < len);
    }


    void SerialPosix::setBaud(int baud) {
        speed_t baudBits;
        switch (baud) {
            case 50:
                baudBits = B50;
                break;
            case 75:
                baudBits = B75;
                break;
            case 110:
                baudBits = B110;
                break;
            case 134:
                baudBits = B134;
                break;
            case 150:
                baudBits = B150;
                break;
            case 200:
                baudBits = B200;
                break;
            case 300:
                baudBits = B300;
                break;
            case 600:
                baudBits = B600;
                break;
            case 1200:
                baudBits = B1200;
                break;
            case 1800:
                baudBits = B1800;
                break;
            case 2400:
                baudBits = B2400;
                break;
            case 4800:
                baudBits = B4800;
                break;
            case 9600:
                baudBits = B9600;
                break;
            case 19200:
                baudBits = B19200;
                break;
            case 38400:
                baudBits = B38400;
                break;
#ifdef B57600 // Not a part of POSIX.1, but supported by Linux 4.15
            case 57600:
                baudBits = B57600;
                break;
            case 115200:
                baudBits = B115200;
                break;
            case 230400:
                baudBits = B230400;
                break;
            case 460800:
                baudBits = B460800;
                break;
            case 500000:
                baudBits = B500000;
                break;
            case 576000:
                baudBits = B576000;
                break;
            case 921600:
                baudBits = B921600;
                break;
            case 1000000:
                baudBits = B1000000;
                break;
            case 1152000:
                baudBits = B1152000;
                break;
            case 1500000:
                baudBits = B1500000;
                break;
            case 2000000:
                baudBits = B2000000;
                break;
            case 2500000:
                baudBits = B2500000;
                break;
            case 3000000:
                baudBits = B3000000;
                break;
            case 3500000:
                baudBits = B3500000;
                break;
            case 4000000:
                baudBits = B4000000;
                break;
#endif
            default:
                throw std::runtime_error("SerialPosix:\tNot a valid baud");
        }
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }
        cfsetospeed(&tty, static_cast<speed_t>(baudBits));
        cfsetispeed(&tty, static_cast<speed_t>(baudBits));

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }
    }

    void SerialPosix::setPort(const std::string &port) {
        if (this->fd >= 0) {
            close(this->fd);
        }

        this->fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
        if (this->fd < 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno) + port);
        }
    }

    void SerialPosix::setParity(Parity parity) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }

        tty.c_cflag &= ~(PARENB | PARODD); // shut off parity

        tcflag_t parityFlag = 0;
        switch (parity) {
            case Parity::NONE:
                parityFlag = 0;
                break;
            case Parity::ODD:
                parityFlag = PARENB | PARODD;
                break;
            case Parity::EVEN:
                parityFlag = PARENB;
                break;
            case Parity::MARK:
                parityFlag = PARENB | PARODD | CMSPAR;
                break;
            case Parity::SPACE:
                parityFlag = PARENB | CMSPAR;
                break;
        }
        tty.c_cflag |= parityFlag;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }
    }

    void SerialPosix::setDataBits(int dataBits) {

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }

        tcflag_t charSize;
        switch (dataBits) {
            case 5:
                charSize = CS5;
                break;
            case 6:
                charSize = CS6;
                break;
            case 7:
                charSize = CS7;
                break;
            case 8:
                charSize = CS8;
                break;
            default:
                throw std::runtime_error("SerialPosix:\tInvalid dataBits (needs to be \\in [5,8]");
        }
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | charSize; // 8-bit chars

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }
    }

    void SerialPosix::setStopBits(int stopBits) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }

        if (stopBits == 1) {
            tty.c_cflag &= ~CSTOPB;
        } else if (stopBits == 2) {
            tty.c_cflag |= CSTOPB;
        } else {
            throw std::runtime_error("SerialPosix:\tstopBits needs to be 1 or 2");
        }

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(std::string{"SerialPosix:\t"} + strerror(errno));
        }
    }

    auto SerialPosix::getChannelIn() -> InputChannel<rcLib::Package> & {
        return in;
    }

    auto SerialPosix::getChannelOut() -> OutputChannel<rcLib::Package> & {
        return out;
    }

    SerialPosix::~SerialPosix() {
        close(this->fd);
    }
} // namespace device
