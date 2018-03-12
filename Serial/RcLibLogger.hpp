//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
#define FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP


#include "RadioControlProtocol/rcLib.hpp"
#include "../Channel.hpp"

class RcLibLogger {
public:
    RcLibLogger(Channel<rcLib::Package> channel, int fd = 1);
};


#endif //FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
