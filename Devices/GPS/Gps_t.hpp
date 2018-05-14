//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_GPS_T_HPP
#define FLIGHTCOMPUTER_GPS_T_HPP


#include <cmath>

class Gps_t {
public:
    explicit Gps_t(double lat, double lon, double altitude = 0) : lat(lat), lon(lon), altitude(altitude){};
    double lat, lon;
    double altitude;

    double distanceTo(Gps_t gps) {
        double phi1 = this->lat / 180.0 * M_PI;
        double phi2 = gps.lat / 180.0 * M_PI;
        double delta_phi = phi2 - phi1;
        double delta_lambda = (gps.lon-this->lon) / 180.0 * M_PI;

        double a = std::sin(delta_phi/2) * std::sin(delta_phi/2) +
                   std::cos(phi1) * std::cos(phi2) *
                   std::sin(delta_lambda/2) * std::sin(delta_lambda/2);
        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));

        return EARTH_RADIUS * c;
    }

    double angleTo(Gps_t gps) {
        Gps_t supportPoint(this->lat, gps.lon);
        double xDiff = this->distanceTo(supportPoint);
        double yDiff = supportPoint.distanceTo(gps);
        return std::atan2(yDiff, xDiff) * 180.0 / M_PI;
    }

private:
    static constexpr double EARTH_RADIUS = 63713;
};


#endif //FLIGHTCOMPUTER_GPS_T_HPP
