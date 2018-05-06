//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_T_HPP
#define FLIGHTCOMPUTER_GPS_T_HPP

#include <cmath>
#include <iostream>

#include "../../Utilities/json.hpp"

using json = nlohmann::json;

class Gps_t {
public:
    explicit Gps_t(json json) {
        this->lat = json["lat"];
        this->lon = json["lon"];
        this->speed = json["speed"];
        this->timestamp = json["timestamp"];
    }

    Gps_t(double lat, double lon) : lat(lat), lon(lon), speed(0), timestamp(0){};

    Gps_t() : lat(0), lon(0), speed(0), timestamp(0){};

    double lat, lon;
    double speed;
    double timestamp;

    friend std::ostream &operator<<(std::ostream &ostream, Gps_t gps) {
        ostream << "Lat: " << gps.lat << "\tLon: " << gps.lon;
        return ostream;
    }

    json toJson() {
        json ret;
        ret["lat"] = lat;
        ret["lon"] = lon;
        ret["speed"] = speed;
        ret["timestamp"] = timestamp;

        return ret;
    }

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
