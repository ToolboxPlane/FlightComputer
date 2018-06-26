//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
#define FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP

#include "../../Channel.hpp"
#include "../rcLib/PackageExtended.hpp"
#include "../../ThreadModule.hpp"
#include "SerialDriver.hpp"
#include "../../MultipleOutputChannel.hpp"

class Serial : public ThreadModule {
public:
    Serial(const std::string& port, int baud);
    void run() override;

    Channel<rcLib::PackageExtended> &getChannelIn();
    MultipleOutputChannel<rcLib::PackageExtended> &getChannelOut();
private:
    SerialDriver serial;
    Channel<rcLib::PackageExtended> in;
    MultipleOutputChannel<rcLib::PackageExtended> out;

};


#endif //FLIGHTCOMPUTERMONOLITHIC_SERIAL_HPP
