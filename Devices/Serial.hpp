//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIAL_HPP
#define FLIGHTCOMPUTER_SERIAL_HPP

#include <string>
#include <termios.h>

class Serial {
public:
    explicit Serial(std::string port, int baud = B9600, bool blocking = false);
    void setBlocking(bool isBlocking);
    void setAttributes(int baud, int parity, int timeoutMs = 500);
    int fd = 0;
    bool ready = false;
};


#endif //FLIGHTCOMPUTER_SERIAL_HPP
