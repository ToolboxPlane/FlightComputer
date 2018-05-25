//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP
#define FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP

#include <cmath>
#include <iostream>

#include "../../Utilities/json.hpp"

#include "Gps_t.hpp"

using json = nlohmann::json;

class GpsMeasurement_t {
public:
    explicit GpsMeasurement_t(json json) : location(0,0){
        this->fixAquired = json["fixAquired"];
        if(this->fixAquired) {
            this->location.lat = json["lat"];
            this->location.lon = json["lon"];
            this->speed = json["speed"];
            this->timestamp = json["timestamp"];
            this->location.altitude = json["altitude"];
            this->climb = json["climb"];
        }
    }

    GpsMeasurement_t(double lat, double lon) :
            location(lat, lon,0), speed(0), timestamp(0), fixAquired(false), climb(0){};

    GpsMeasurement_t() : location(0,0), speed(0), timestamp(0), fixAquired(false), climb(0){};

    friend std::ostream &operator<<(std::ostream &ostream, GpsMeasurement_t gps) {
        if(!gps.fixAquired) {
            ostream << "No fix";
        } else {
            ostream << "Timestamp: " << gps.timestamp <<
                    "\tLat: " << gps.location.lat <<
                    "\tLon: " << gps.location.lon <<
                    "\tAlt: " << gps.location.altitude <<
                    "\tClimb: " << gps.climb;
        }
        return ostream;
    }

    json toJson() {
        json ret;
        ret["fixAquired"] = fixAquired;
        if(fixAquired) {
            ret["altitude"] = location.altitude;
            ret["climb"] = climb;
            ret["lat"] = location.lat;
            ret["lon"] = location.lon;
            ret["speed"] = speed;
            ret["timestamp"] = timestamp;
        }

        return ret;
    }

    bool fixAquired;

    Gps_t location;
    double speed;
    double timestamp;
    double climb;


};

#endif //FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP
