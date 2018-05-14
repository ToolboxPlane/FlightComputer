//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_HPP
#define FLIGHTCOMPUTER_GPS_HPP

#include <string>
#include <gps.h>

#include "GpsMeasurement_t.hpp"
#include "../../ThreadModule.hpp"
#include "../../Channel.hpp"
#include "../Serial.hpp"

class Gps : ThreadModule{
public:
    Gps();

    Channel<GpsMeasurement_t> &getChannelOut();
private:
    void run() override;
    Channel<GpsMeasurement_t> out;
    gps_data_t gps_data;
};


#endif //FLIGHTCOMPUTER_GPS_HPP
