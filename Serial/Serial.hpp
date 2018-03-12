//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "string"
#include "Channel.hpp"

class Serial {
public:
    Serial(const std::string& port, int baud, Channel in, Channel out);
    void run();

    void setBlocking(bool isBlocking);
    void setAttributes(int baud, int parity);
private:
    int fd;
    Channel in, out;

};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
