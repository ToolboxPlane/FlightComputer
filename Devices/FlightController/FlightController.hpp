//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "../../Channel.hpp"
#include "../rcLib/PackageExtended.hpp"
#include "../../ThreadModule.hpp"
#include "../Serial.hpp"
#include "../../MultipleOutputChannel.hpp"

class FlightController : public ThreadModule {
public:
    FlightController(const std::string& port, int baud);
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getChannelOut();
private:
    Serial serial;
    Channel<rcLib::PackageExtended> in;
    MultipleOutputChannel<rcLib::PackageExtended> out;

};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
