//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_GPS_T_HPP
#define FLIGHTCOMPUTER_GPS_T_HPP


#include <cmath>
#include "../../../Utilities/Si/SiLiterals.hpp"
#include "../../../constants.hpp"

class Gps_t {
    public:
        Gps_t(double lat, double lon, si::base::Meter<> altitude = 0 * si::base::meter)
                : lat(lat), lon(lon), altitude(altitude) {};
        double lat, lon;
        si::base::Meter<> altitude;

        si::base::Meter<> distanceTo(const Gps_t &gps) const {
            auto phi1 = this->lon / 180 * M_PI;
            auto sigma1 = M_PI / 2 - (this->lat / 180 * M_PI);
            auto r1 = EARTH_RADIUS + this->altitude;
            auto x1 = r1 * std::sin(sigma1) * std::cos(phi1);
            auto y1 = r1 * std::sin(sigma1) * std::sin(phi1);
            auto z1 = r1 * std::cos(sigma1);

            double phi2 = gps.lon / 180 * M_PI;
            double sigma2 = M_PI / 2 - (gps.lat / 180 * M_PI);
            auto r2 = EARTH_RADIUS + gps.altitude;
            auto x2 = r2 * std::sin(sigma2) * std::cos(phi2);
            auto y2 = r2 * std::sin(sigma2) * std::sin(phi2);
            auto z2 = r2 * std::cos(sigma2);

            auto dx = x1 - x2;
            auto dy = y1 - y2;
            auto dz = z1 - z2;

            return std::sqrt(static_cast<si::default_type>(dx * dx + dy * dy + dz * dz)) * si::base::meter;
        }

        auto angleTo(const Gps_t &gps) const -> si::default_type {
            Gps_t supportPoint(this->lat, gps.lon);
            auto xDiff = this->distanceTo(supportPoint);
            auto yDiff = supportPoint.distanceTo(gps);
            return static_cast<si::default_type>(-std::atan2(static_cast<decltype(yDiff)::type>(yDiff),
                                                             static_cast<decltype(xDiff)::type>(xDiff)) * 180.0 / M_PI +
                                                 90);
        }

        static constexpr si::base::Meter<> EARTH_RADIUS{EARTH_CIRCUMFERENCE / (2 * M_PI)};
};

#endif //FLIGHTCOMPUTER_GPS_T_HPP
