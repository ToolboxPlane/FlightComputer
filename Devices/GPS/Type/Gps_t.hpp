//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_GPS_T_HPP
#define FLIGHTCOMPUTER_GPS_T_HPP


#include <SiBase.hpp>
#include <SiStl.hpp>
#include <cmath>

#include "../../../constants.hpp"

class Gps_t {
  public:
    Gps_t(double lat, double lon, si::Meter<> altitude = 0.0F * si::meter) : lat(lat), lon(lon), altitude(altitude){};
    double lat, lon;
    si::Meter<> altitude;

    si::Meter<> distanceTo(const Gps_t &gps) const {
        auto latDiff = gps.lat - this->lat;
        auto lonDiff = gps.lon - this->lon;
        auto latMean = (gps.lat + this->lat) / 2;
        auto altDiff = static_cast<double>(gps.altitude - this->altitude);

        auto latDist = latDiff / 360 * EARTH_CIRCUMFERENCE;
        auto lonDist = lonDiff / 360 * EARTH_CIRCUMFERENCE * std::cos(latMean / 180 * M_PI);

        return si::Meter<>{static_cast<float>(std::sqrt(latDist * latDist + lonDist * lonDist + altDiff * altDiff))};
    }

    auto angleTo(const Gps_t &gps) const -> si::Scalar<> {
        auto latDiff = gps.lat - this->lat;
        auto lonDiff = gps.lon - this->lon;
        auto latMean = (gps.lat + this->lat) / 2;

        auto latDist = latDiff;
        auto lonDist = lonDiff * std::cos(latMean / 180 * M_PI);

        auto angle = -std::atan2(latDist, lonDist) * 180.0 / M_PI + 90;
        angle = std::fmod(angle, 360);
        if (angle > 180) {
            angle -= 360;
        } else if (angle < -180) {
            angle += 360;
        }
        return angle;
    }

    static constexpr si::Meter<> EARTH_RADIUS{static_cast<float>(EARTH_CIRCUMFERENCE) / (2 * M_PI_F)};
};

#endif // FLIGHTCOMPUTER_GPS_T_HPP
