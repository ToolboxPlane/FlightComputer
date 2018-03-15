//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPSDEBUG_HPP
#define FLIGHTCOMPUTER_GPSDEBUG_HPP


#include "../../ThreadModule.hpp"
#include "gps_t.hpp"
#include "../../Channel.hpp"

class GpsDebug : ThreadModule{
public:
    explicit GpsDebug(std::string tag) : tag(std::move(tag)), ThreadModule() {}

    Channel<gps_t> &getChannelIn();
private:
    std::string tag;
    Channel<gps_t> in;
    void run() override;
};


#endif //FLIGHTCOMPUTER_GPSDEBUG_HPP
