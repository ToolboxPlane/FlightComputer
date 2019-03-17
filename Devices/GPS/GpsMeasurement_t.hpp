//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP
#define FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Gps_t.hpp"

class GpsMeasurement_t {
public:
    explicit GpsMeasurement_t(const std::vector<std::string> &items) : location(0,0){
        this->fixAquired = static_cast<bool>(std::stoi(items[0]));
        this->location.lat = std::stod(items[1]);
        this->location.lon = std::stod(items[2]);
        this->speed = std::stod(items[3]);
        this->timestamp = std::stoi(items[4]);
        this->location.altitude = std::stod(items[5]);
        this->climb = std::stod(items[6]);
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


    std::string getLine() {
        std::stringstream stringstream;
        stringstream << (this->fixAquired ? 1:0) << "; ";
        stringstream << this->location.lat << "; ";
        stringstream << this->location.lon << "; ";
        stringstream << this->speed << "; ";
        stringstream << this->timestamp << "; ";
        stringstream << this->location.altitude << "; ";
        stringstream << this->climb;

        return stringstream.str();
    }

    static std::string header() {
        return "Fix; Lat; Lon; Speed; Timestamp; Altitude; Climb";
    }

    bool fixAquired;

    Gps_t location;
    double speed;
    double timestamp;
    double climb;
};

#endif //FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP
