//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
#define FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP


#include "RadioControlProtocol/rcLib.hpp"
#include "../Channel.hpp"
#include <ostream>

class RcLibLogger {
public:
    explicit RcLibLogger() = default;
    void run();
    Channel<rcLib::Package> &getChannelIn();

private:
    Channel<rcLib::Package> channel;
};


#endif //FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
