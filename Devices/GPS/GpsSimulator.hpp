//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPSSIMULATOR_HPP
#define FLIGHTCOMPUTER_GPSSIMULATOR_HPP


#include "../../ThreadModule.hpp"
#include "../../Channel.hpp"
#include "GpsMeasurement_t.hpp"
#include "../../MultipleOutputChannel.hpp"

class GpsSimulator : public ThreadModule{
public:
    GpsSimulator() : ThreadModule() {}

    MultipleOutputChannel<GpsMeasurement_t> &getChannelOut();
private:
    MultipleOutputChannel<GpsMeasurement_t> out;
    void run() override;
};


#endif //FLIGHTCOMPUTER_GPSSIMULATOR_HPP
