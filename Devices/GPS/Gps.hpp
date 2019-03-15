//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_HPP
#define FLIGHTCOMPUTER_GPS_HPP

#include <string>
#include <gps.h>

#include "GpsMeasurement_t.hpp"
#include "../../Filter.hpp"
#include "../../InputChannel.hpp"
#include "../../OutputChannel.hpp"

class Gps : public Filter{
public:
    Gps();

    OutputChannel<GpsMeasurement_t> &getChannelOut();
private:
    void run() override;
    OutputChannel<GpsMeasurement_t> out;
    gps_data_t gps_data;
};


#endif //FLIGHTCOMPUTER_GPS_HPP
