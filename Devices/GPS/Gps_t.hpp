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
        double phi1 = this->lon / 180 * M_PI;
        double sigma1 = M_PI/2 - (this->lat / 180 * M_PI);
        double r1 = EARTH_RADIUS + this->altitude;
        double x1 = r1 * std::sin(sigma1) * std::cos(phi1);
        double y1 = r1 * std::sin(sigma1) * std::sin(phi1);
        double z1 = r1 * std::cos(sigma1);

        double phi2 = gps.lon / 180 * M_PI;
        double sigma2 = M_PI/2 - (gps.lat / 180 * M_PI);
        double r2 = EARTH_RADIUS + gps.altitude;
        double x2 = r2 * std::sin(sigma2) * std::cos(phi2);
        double y2 = r2 * std::sin(sigma2) * std::sin(phi2);
        double z2 = r2 * std::cos(sigma2);

        double dx = x1 - x2;
        double dy = y1 - y2;
        double dz = z1 - z2;

        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }

    double angleTo(Gps_t gps) {
        Gps_t supportPoint(this->lat, gps.lon);
        double xDiff = this->distanceTo(supportPoint);
        double yDiff = supportPoint.distanceTo(gps);
        return -std::atan2(yDiff, xDiff) * 180.0 / M_PI + 90;
    }

private:
    static constexpr double EARTH_RADIUS = 6371300;
};


#endif //FLIGHTCOMPUTER_GPS_T_HPP
