//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_HPP
#define FLIGHTCOMPUTER_GPS_HPP

#include <string>

#include "gps_t.hpp"
#include "../../ThreadModule.hpp"

class Gps : ThreadModule{
public:
    Gps(std::string port, int baud);

    Channel<gps_t> &getChannelOut();
private:
    Channel<gps_t> out;
};


#endif //FLIGHTCOMPUTER_GPS_HPP
