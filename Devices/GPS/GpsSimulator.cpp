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
        gps.location.lon = 48.4235495;
        gps.location.lat = 9.9292937;
        gps.timestamp = 24;
        gps.location.altitude = 500;
        gps.fixAquired = true;
        gps.speed = 40;
        gps.climb = 0;
        out.put(gps);
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}

#pragma clang diagnostic pop