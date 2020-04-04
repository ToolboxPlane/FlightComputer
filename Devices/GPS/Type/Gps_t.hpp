//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_GPS_T_HPP
#define FLIGHTCOMPUTER_GPS_T_HPP


#include <cmath>
#include "../../../Utilities/Si/SiLiterals.hpp"
#include "../../../Utilities/Si/SiStl.hpp"
#include "../../../Utilities/Si/SiExtended.hpp"
#include "../../../constants.hpp"

class Gps_t {
    public:
        Gps_t(double lat, double lon, si::Meter<> altitude = 0 * si::meter)
                : lat(lat), lon(lon), altitude(altitude) {};
        double lat, lon;
        si::Meter<> altitude;

        si::Meter<> distanceTo(const Gps_t &gps) const {
            auto phi1 = static_cast<si::default_type>(this->lon / 180 * M_PI_F);
            auto sigma1 = M_PI_F / 2 - static_cast<si::default_type>(this->lat / 180 * M_PI_F);
            auto r1 = EARTH_RADIUS + this->altitude;
            auto x1 = r1 * std::sin(sigma1) * std::cos(phi1);
            auto y1 = r1 * std::sin(sigma1) * std::sin(phi1);
            auto z1 = r1 * std::cos(sigma1);

            auto phi2 = static_cast<si::default_type>(gps.lon / 180 * M_PI_F);
            auto sigma2 = M_PI_F / 2 - static_cast<si::default_type>(gps.lat / 180 * M_PI_F);
            auto r2 = EARTH_RADIUS + gps.altitude;
            auto x2 = r2 * std::sin(sigma2) * std::cos(phi2);
            auto y2 = r2 * std::sin(sigma2) * std::sin(phi2);
            auto z2 = r2 * std::cos(sigma2);

            auto dx = x1 - x2;
            auto dy = y1 - y2;
            auto dz = z1 - z2;

            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }

        auto angleTo(const Gps_t &gps) const -> si::default_type {
            Gps_t supportPoint(this->lat, gps.lon, (this->altitude + gps.altitude) / 2);
            auto xDiff = this->distanceTo(supportPoint);
            auto yDiff = supportPoint.distanceTo(gps);

            std::cout << "this=(" << this->lat << ","  << this->lon << ")\n";
            std::cout << "gps=(" << gps.lat << ","  << gps.lon << ")\n";
            std::cout << "support=(" << supportPoint.lat << ","  << supportPoint.lon << ")\n";
            std::cout << "xDiff=" << xDiff << "\tyDiff=" << yDiff << std::endl;

            return -std::atan2(yDiff, xDiff) * 180.0 / M_PI + 90;
        }

        static constexpr si::Meter<> EARTH_RADIUS{EARTH_CIRCUMFERENCE / (2 * M_PI_F)};
};

#endif //FLIGHTCOMPUTER_GPS_T_HPP
