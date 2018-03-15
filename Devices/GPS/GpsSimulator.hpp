//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPSSIMULATOR_HPP
#define FLIGHTCOMPUTER_GPSSIMULATOR_HPP


#include "../../ThreadModule.hpp"
#include "../../Channel.hpp"
#include "gps_t.hpp"

class GpsSimulator : public ThreadModule{
public:
    GpsSimulator() : ThreadModule() {}

    Channel<gps_t> &getChannelOut();
private:
    Channel<gps_t> out;
    void run() override;
};


#endif //FLIGHTCOMPUTER_GPSSIMULATOR_HPP