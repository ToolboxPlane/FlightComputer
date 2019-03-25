//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "GpsSimulator.hpp"
#include "../../Utilities/Si/SiLiterals.hpp"

namespace device {
    using namespace si::literals;
    using namespace si::extended;

    GpsSimulator::GpsSimulator(const int intervalMs) : intervalMs(intervalMs) {
        this->start();
    }

    void GpsSimulator::run() {
        while (true) {
            GpsMeasurement_t gps{};
            gps.location.lon = 48.4235495;
            gps.location.lat = 9.9292937;
            gps.timestamp = 24_second;
            gps.location.altitude = 500_meter;
            gps.fixAquired = true;
            gps.speed = 40_speed;
            gps.climb = 0_speed;
            out.put(gps);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
    }

    auto GpsSimulator::getChannelOut() -> OutputChannel<GpsMeasurement_t> & {
        return out;
    }
}
