//
// Created by paul on 12.03.18.
//

#include <fcntl.h>
#include <bits/ios_base.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <termios.h>

#include "Serial.hpp"

namespace device {
    Serial::Serial(const std::string &port, int baud, const int BUF_SIZE)
            : StreamDevice(BUF_SIZE) {
        fileDescriptor = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
        if (fileDescriptor < 0) {
            throw std::ios_base::failure("Error opening the serial port!");
        }
        setBlocking(false);
        setAttributes(baud, 0);
        this->start();
    }

    void Serial::setBlocking(bool isBlocking) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fileDescriptor, &tty) != 0) {
            throw std::ios_base::failure("Error from tggettattr");
        }

        tty.c_cc[VMIN] = static_cast<cc_t>(isBlocking);
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr(fileDescriptor, TCSANOW, &tty) != 0) {
            throw std::ios_base::failure("Error setting term attributes");
        }
    }

    void Serial::setAttributes(int baud, int parity, int timeoutMs) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fileDescriptor, &tty) != 0) {
            throw std::ios_base::failure("Error from tggettattr");
        }

        cfsetospeed(&tty, static_cast<speed_t>(baud));
        cfsetispeed(&tty, static_cast<speed_t>(baud));

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN] = 0;            // read doesn't block
        tty.c_cc[VTIME] = static_cast<cc_t>(timeoutMs / 100);            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr(fileDescriptor, TCSANOW, &tty) != 0) {
            throw std::ios_base::failure("Error setting term attributes");
        }
    }

    int Serial::getFileDescriptor() {
        return fileDescriptor;
    }

    void Serial::invalidateFileDescriptor() {

    }
}