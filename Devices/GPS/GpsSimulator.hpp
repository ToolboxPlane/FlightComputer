//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPSSIMULATOR_HPP
#define FLIGHTCOMPUTER_GPSSIMULATOR_HPP


#include "../../Filter.hpp"
#include "../../Channel.hpp"
#include "GpsMeasurement_t.hpp"
#include "../../MultipleOutputChannel.hpp"

class GpsSimulator : public Filter{
public:
    explicit GpsSimulator(int intervalMs = 500);

    MultipleOutputChannel<GpsMeasurement_t> &getChannelOut();
private:
    MultipleOutputChannel<GpsMeasurement_t> out;
    void run() override;
    const int intervalMs;
};


#endif //FLIGHTCOMPUTER_GPSSIMULATOR_HPP
