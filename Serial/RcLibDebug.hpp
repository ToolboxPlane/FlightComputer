//
// Created by paul on 12.03.18.
//

#ifndef FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
#define FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP


#include "RadioControlProtocol/rcLib.hpp"
#include "../Channel.hpp"
#include "../ThreadModule.hpp"
#include <ostream>

class RcLibDebug : ThreadModule{
public:
    explicit RcLibDebug(const std::string &tag = "RC-LIB");
    void run() override;
    Channel<rcLib::Package> &getChannelIn();
private:
    Channel<rcLib::Package> channel;
    std::string tag;
};


#endif //FLIGHTCOMPUTER_SERIALPORTLOGGER_HPP
