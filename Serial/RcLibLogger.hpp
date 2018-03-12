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
    explicit RcLibLogger(Channel<rcLib::Package> &channel);
    void run();

private:
    Channel<rcLib::Package> &channel;
};


#endif //FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
