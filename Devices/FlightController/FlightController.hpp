//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "string"
#include "../../Channel.hpp"
#include "../RadioControlProtocol/rcLib.hpp"
#include "../../ThreadModule.hpp"
#include <termios.h>
#include "../Serial.hpp"

class FlightController : public ThreadModule {
public:
    FlightController(const std::string& port, int baud);
    void run() override;

    Channel<rcLib::Package> &getChannelIn();
    Channel<rcLib::Package> &getChannelOut();
private:
    Serial serial;
    Channel<rcLib::Package> in, out;

};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
