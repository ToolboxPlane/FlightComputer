//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "string"
#include "../../Channel.hpp"
#include "../rcLib/PackageExtended.hpp"
#include "../../ThreadModule.hpp"
#include <termios.h>
#include "../Serial.hpp"

class FlightController : public ThreadModule {
public:
    FlightController(const std::string& port, int baud);
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    Channel<rcLib::PackageExtended> &getChannelOut();
private:
    Serial serial;
    Channel<rcLib::PackageExtended> in, out;

};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
