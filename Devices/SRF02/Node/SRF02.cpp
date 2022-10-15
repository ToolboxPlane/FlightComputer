/**
 * @file SRF02.cpp
 * @author paul
 * @date 14.03.20
 * Description here TODO
 */
#include "SRF02.hpp"

#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <zconf.h>

namespace device {
    device::SRF02::SRF02(const std::string &port) {
        fd = open(port.c_str(), O_RDWR | O_NOCTTY);
        if (this->fd < 0) {
            throw std::runtime_error(std::string{"SRF02:\t"} + strerror(errno));
        }

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(std::string{"SRF02:\t"} + strerror(errno));
        }

        cfsetispeed(&tty, B19200); // Set the baud rates to 19200
        cfmakeraw(&tty);
        tty.c_cc[VMIN] = 1;
        tty.c_cc[VTIME] = 50;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(std::string{"SRF02:\t"} + strerror(errno));
        }

        start();
    }

    auto SRF02::getChannelOut() -> OutputChannel<si::Meter<>> & {
        return out;
    }

    void SRF02::run() {
        using namespace std::chrono_literals;
        while (true) {
            // USB-I2C Adapter documentation: http://www.robot-electronics.co.uk/htm/usb_i2c_tech.htm
            // SRF02 I2C Mode documentation: https://www.robot-electronics.co.uk/htm/srf02techI2C.htm
            // Write byte 0x51 to register 0x00 on device at I2C address 0xE0 ("start ranging in cm")
            sendBuff({0x55, 0xE0, 0x00, 0x01, 0x51});
            uint8_t confirmation = 0;
            if (read(this->fd, &confirmation, 1) != 1 || confirmation == 0) {
                std::cerr << "SRF02:\tInvalid confirmation (" << static_cast<int>(confirmation) << ")" << std::endl;
                std::this_thread::sleep_for(1.0s);
                continue;
            }

            std::this_thread::sleep_for(66ms);

            // Read 2 bytes from register 0x02 on device at 0xE1 (ranging result)
            sendBuff({0x55, 0xE1, 0x02, 0x02});
            std::array<uint8_t, 2> data{};
            auto readed = read(this->fd, data.data(), data.size());
            if (readed == static_cast<ssize_t>(data.size())) {
                float distance = data[0] << 8u | data[1];

                out.put(distance / 100.0F * si::meter);
            } else {
                std::cerr << "SRF02:\tread: invalid response" << std::endl;
            }
        }
    }

    void SRF02::sendBuff(const std::vector<uint8_t> &buffer) {
        std::size_t written = 0;
        do {
            auto result = write(this->fd, buffer.data() + written, buffer.size() - written);
            if (result < 0) {
                std::cerr << "SRF02:\t" << strerror(errno) << std::endl;
            } else {
                written += static_cast<std::size_t>(result);
            }
        } while (written < buffer.size());
    }
} // namespace device
