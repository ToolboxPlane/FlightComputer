//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "GpsDebug.hpp"

Channel<gps_t> &GpsDebug::getChannelIn() {
    return in;
}

void GpsDebug::run() {
    gps_t gps{};
    while (!in.isClosed()) {
        if(in.get(gps)) {
            std::cout << "["  << tag << "]\t";
            std::cout << "Time: " << gps.timestamp << "\t";
            std::cout << "Lat: " << gps.lat << "\t";
            std::cout << "Lon: " << gps.lon << std::endl;
        }
    }
}
