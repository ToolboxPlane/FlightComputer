//
// Created by paul on 15.03.18.
//

#include "GpsSimulator.hpp"

#include <random>

namespace device {
    using namespace si::literals;
    using namespace si;

    GpsSimulator::GpsSimulator(const int intervalMs) : intervalMs(intervalMs) {
        this->start();
    }

    void GpsSimulator::run() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<si::default_type> sigmaDist(3, 50);
        std::normal_distribution<si::default_type> noiseDist;

        while (true) {
            auto sigmaLat = sigmaDist(rng);
            auto sigmaLon = sigmaDist(rng);
            auto sigmaVert = sigmaDist(rng);
            auto sigmaSpeed = sigmaDist(rng);
            auto sigmaClimb = sigmaDist(rng);


            GpsMeasurement_t gps{};
            auto noiseLat = (noiseDist(rng) * sigmaLat) / static_cast<float>(EARTH_CIRCUMFERENCE) * 360;
            gps.location.lat = 48.4235495 + noiseLat;
            auto noiseLon = (noiseDist(rng) * sigmaLon) / static_cast<float>(EARTH_CIRCUMFERENCE) * 360 /
                            std::cos(gps.location.lat / 180 * M_PI);
            gps.location.lon = 9.9292937 + noiseLon;
            gps.timestamp = si::Second<long double>{0};
            gps.location.altitude = 500_meter + noiseDist(rng) * sigmaVert * meter;
            gps.fixAquired = true;
            gps.speed = 0_speed + noiseDist(rng) * sigmaSpeed * speed;
            gps.climb = 0_speed + noiseDist(rng) * sigmaSpeed * speed;
            gps.epLat = 2_meter * sigmaLat;
            gps.epLon = 2_meter * sigmaLon;
            gps.epVert = 2_meter * sigmaVert;
            gps.epSpeed = 2_speed * sigmaSpeed;
            gps.epClimb = 2_speed * sigmaClimb;
            out.put(gps);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
    }

    auto GpsSimulator::getChannelOut() -> OutputChannel<GpsMeasurement_t> & {
        return out;
    }
} // namespace device
