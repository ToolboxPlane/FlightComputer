//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPSSIMULATOR_HPP
#define FLIGHTCOMPUTER_GPSSIMULATOR_HPP


#include "../../Filter.hpp"
#include "../../InputChannel.hpp"
#include "GpsMeasurement_t.hpp"
#include "../../OutputChannel.hpp"

class GpsSimulator : public Filter{
public:
    explicit GpsSimulator(int intervalMs = 500);

    OutputChannel<GpsMeasurement_t> &getChannelOut();
private:
    OutputChannel<GpsMeasurement_t> out;
    void run() override;
    const int intervalMs;
};


#endif //FLIGHTCOMPUTER_GPSSIMULATOR_HPP
