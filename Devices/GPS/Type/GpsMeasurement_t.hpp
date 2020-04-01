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
#include "../../../Utilities/Si/SiExtended.hpp"

class GpsMeasurement_t {
    public:
        GpsMeasurement_t(const std::vector<std::string> &items) : location{0, 0} {
            this->fixAquired = static_cast<bool>(std::stoi(items[0]));
            this->location.lat = std::stod(items[1]);
            this->location.lon = std::stod(items[2]);
            this->speed = std::stof(items[3]) * si::extended::speed;
            this->timestamp = std::stoi(items[4]) * si::base::second;
            this->location.altitude = std::stof(items[5]) * si::base::meter;
            this->climb = std::stof(items[6]) * si::extended::speed;
        }

        GpsMeasurement_t(double lat, double lon) :
                fixAquired{false}, location{lat, lon, 0}, speed{0}, timestamp{0}, climb{0} {};

        GpsMeasurement_t() : fixAquired{false}, location{0, 0}, speed{0}, timestamp{0}, climb{0},
                             epLat{std::numeric_limits<si::default_type>::infinity()},
                             epLon{std::numeric_limits<si::default_type>::infinity()},
                             epVert{std::numeric_limits<si::default_type>::infinity()},
                             epSpeed{std::numeric_limits<si::default_type>::infinity()},
                             epClimb{std::numeric_limits<si::default_type>::infinity()} {};

        friend std::ostream &operator<<(std::ostream &ostream, GpsMeasurement_t gps) {
            if (!gps.fixAquired) {
                ostream << "No fix";
            } else {
                ostream << "Timestamp: " << gps.timestamp <<
                        "\tLat: " << gps.location.lat <<
                        "\tLon: " << gps.location.lon <<
                        "\tAlt: " << gps.location.altitude <<
                        "\tClimb: " << gps.climb <<
                        "\tSpeed: " << gps.speed;
            }
            return ostream;
        }


        auto getLine() const -> std::string {
            std::stringstream stringstream;
            stringstream << (this->fixAquired ? 1 : 0) << "; ";
            stringstream << this->location.lat << "; ";
            stringstream << this->location.lon << "; ";
            stringstream << this->speed << "; ";
            stringstream << this->timestamp << "; ";
            stringstream << this->location.altitude << "; ";
            stringstream << this->climb;

            return stringstream.str();
        }

        static auto header() -> std::string {
            return "Fix; Lat; Lon; Speed; Timestamp; Altitude; Climb";
        }

        bool fixAquired;

        Gps_t location;
        si::extended::Speed<> speed;
        si::base::Second<> timestamp;
        si::extended::Speed<> climb;
        si::base::Meter<> epLat, epLon, epVert;
        si::extended::Speed<> epSpeed, epClimb;
};

#endif //FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP
