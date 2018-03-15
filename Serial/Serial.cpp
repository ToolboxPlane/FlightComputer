//
// Created by paul on 12.03.18.
//

#include <fcntl.h>
#include <bits/ios_base.h>
#include <termio.h>
#include <cstring>
#include <unistd.h>

#include "Serial.hpp"

#define BUF_SIZE 64

Serial::Serial(const std::string& port, int baud)
        : in(), out(), ThreadModule() {
    this->fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if(this->fd < 0) {
        throw std::ios_base::failure("Error opening the serial port!");
    }
    this->setBlocking(false);
    this->setAttributes(baud, 0);
}

void Serial::run() {
    rcLib::Package toSend;
    rcLib::Package received;
    uint8_t buf[BUF_SIZE];

    while(!in.isClosed() && !out.isClosed()) {
        ssize_t readed = read(this->fd, buf, sizeof(buf));
        for(auto c=0; c<readed; c++) {
            if(received.decode(buf[c])) {
                out.put(received);
            }
        }

        if(in.get(toSend, false)) {
            size_t length = toSend.encode();
            ssize_t written = 0;
            do {
                ssize_t result = write(this->fd, toSend.getEncodedData(), length);
                if(result < 0) {
                    throw std::ios_base::failure("Error sending data via Serial");
                }
                written += result;
            } while (written < length);
        }
    }
}

void Serial::setBlocking(bool isBlocking) {
    termios tty{};
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        throw std::ios_base::failure("Error from tggettattr");
    }

    tty.c_cc[VMIN]  = static_cast<cc_t>(isBlocking);
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        throw std::ios_base::failure("Error setting term attributes");
    }
}

void Serial::setAttributes(int baud, int parity) {
    termios tty{};
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        throw std::ios_base::failure("Error from tggettattr");
    }

    cfsetospeed (&tty, static_cast<speed_t>(baud));
    cfsetispeed (&tty, static_cast<speed_t>(baud));

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        throw std::ios_base::failure("Error setting term attributes");
    }
}

Channel<rcLib::Package> &Serial::getChannelIn() {
    return this->in;
}

Channel<rcLib::Package> &Serial::getChannelOut() {
    return this->out;
}
