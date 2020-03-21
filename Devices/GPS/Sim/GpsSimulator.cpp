//
// Created by paul on 15.03.18.
//

#include <random>
#include "GpsSimulator.hpp"

namespace device {
    using namespace si::literals;
    using namespace si::extended;
    using namespace si::base;

    GpsSimulator::GpsSimulator(const int intervalMs) : intervalMs(intervalMs) {
        this->start();
    }

    void GpsSimulator::run() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<double> sigmaDist(3, 50);
        std::normal_distribution<double> noiseDist;

        while (true) {
            auto sigmaLat = sigmaDist(rng);
            auto sigmaLon = sigmaDist(rng);
            auto sigmaVert = sigmaDist(rng);
            auto sigmaSpeed = sigmaDist(rng);
            auto sigmaClimb = sigmaDist(rng);


            GpsMeasurement_t gps{};
            auto noiseLat = (noiseDist(rng) * sigmaLat) / EARTH_DIAMETER * 360;
            gps.location.lat = 48.4235495 + noiseLat;
            auto noiseLon = (noiseDist(rng) * sigmaLon) / EARTH_DIAMETER * 360 / std::cos(gps.location.lat / 180 * M_PI);
            gps.location.lon = 9.9292937 + noiseLon;
            gps.timestamp = 0_second;
            gps.location.altitude = 500_meter + noiseDist(rng) * sigmaVert * meter;
            gps.fixAquired = true;
            gps.speed = 0_speed + noiseDist(rng) * sigmaSpeed * speed;
            gps.climb = 0_speed + noiseDist(rng) * sigmaSpeed * speed;
            gps.epLat = 2 * sigmaLat;
            gps.epLon = 2 * sigmaLon;
            gps.epVert = 2 * sigmaVert;
            gps.epSpeed = 2 * sigmaSpeed;
            gps.epVert = 2 * sigmaVert;
            gps.epClimb = 2 * sigmaClimb;
            out.put(gps);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
    }

    auto GpsSimulator::getChannelOut() -> OutputChannel<GpsMeasurement_t> & {
        return out;
    }
}
