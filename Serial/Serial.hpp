//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "string"
#include "../Channel.hpp"
#include "RadioControlProtocol/rcLib.hpp"
#include "../ThreadModule.hpp"
#include <termios.h>

class Serial : public ThreadModule {
public:
    Serial(const std::string& port, int baud);
    void run() override;

    void setBlocking(bool isBlocking);
    void setAttributes(int baud, int parity);
    Channel<rcLib::Package> &getChannelIn();
    Channel<rcLib::Package> &getChannelOut();
private:
    int fd;
    Channel<rcLib::Package> in, out;

};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
