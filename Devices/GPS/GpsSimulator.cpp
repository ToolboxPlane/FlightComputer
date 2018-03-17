//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "GpsSimulator.hpp"

Channel<Gps_t> &GpsSimulator::getChannelOut() {
    return out;
}

void GpsSimulator::run() {
    while(!out.isClosed()) {
        Gps_t gps{};
        gps.lon = 17;
        gps.lat = 23;
        gps.timestamp = 24;
        out.put(gps);
        std::this_thread::sleep_for(std::chrono_literals::operator""ms(500));
    }
}
