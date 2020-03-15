/**
 * @file SRF02.cpp
 * @author paul
 * @date 14.03.20
 * Description here TODO
 */
#include <fcntl.h>
#include <cstring>
#include <termios.h>
#include <zconf.h>
#include "SRF02.hpp"

namespace device {
    device::SRF02::SRF02(const std::string &port) {
        fd = open(port.c_str(), O_RDWR | O_NOCTTY);
        if (this->fd < 0) {
            throw std::runtime_error(std::string{"SRF02:\t"} + strerror(errno));
        }

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
        cfsetospeed(&tty, B19200);
        cfsetispeed(&tty, B19200);
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        tty.c_cflag |= CSTOPB; // Two stop bits

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        start();
    }

    auto SRF02::getChannelOut() -> OutputChannel<si::base::Meter<>> {
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
                std::cerr << "Invalid confirmation (" << static_cast<int>(confirmation) << ")" << std::endl;
                std::this_thread::sleep_for(1.0s);
                continue;
            }

            std::this_thread::sleep_for(75ms);

            // Read 2 bytes from register 0x02 on device at 0xE1 (ranging result)
            sendBuff({0x55, 0xE1, 0x02, 0x02});
            std::array<uint8_t, 2> data{};
            if (read(this->fd, data.data(), data.size()) == data.size()) {
                float distance = data[0] << 8u | data[1];

                out.put(distance/100.0F * si::base::meter);
            }
        }
    }

    void SRF02::sendBuff(const std::vector<uint8_t> &buffer) {
        std::size_t written = 0;
        do {
            auto result = write(this->fd, buffer.data() + written, buffer.size() - written);
            if (result < 0) {
                throw std::runtime_error(strerror(errno));
            }
            written += static_cast<std::size_t>(result);
        } while (written < buffer.size());
    }
}
