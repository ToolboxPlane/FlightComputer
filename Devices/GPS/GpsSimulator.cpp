//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "GpsSimulator.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

GpsSimulator::GpsSimulator(const int intervalMs): intervalMs(intervalMs) {
    this->start();
}

MultipleOutputChannel<GpsMeasurement_t> &GpsSimulator::getChannelOut() {
    return out;
}

void GpsSimulator::run() {
    while(true) {
        GpsMeasurement_t gps{};
        gps.location.lon = 17;
        gps.location.lat = 23;
        gps.timestamp = 24;
        gps.location.altitude = 43;
        gps.fixAquired = true;
        gps.speed = 0;
        gps.climb = 12;
        out.put(gps);
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}

#pragma clang diagnostic pop