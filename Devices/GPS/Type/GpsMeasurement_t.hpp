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
#include "../../../Utilities/Recording/Serialization.hpp"

class GpsMeasurement_t {
    public:
        GpsMeasurement_t(double lat, double lon) :
                fixAquired{false}, location{lat, lon, 0 * si::meter}, speed{0},
                timestamp{0}, climb{0} {};

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

        bool fixAquired;

        Gps_t location;
        si::Speed<> speed;
        si::Second<long double> timestamp;
        si::Speed<> climb;
        si::Meter<> epLat{}, epLon{}, epVert{};
        si::Speed<> epSpeed{}, epClimb{};
};

namespace recording {
    template <>
    auto header<GpsMeasurement_t>() -> std::string;

    template <>
    auto getLine<GpsMeasurement_t>(const GpsMeasurement_t &gpsMeasurement) -> std::string;

    template<>
    auto getItem<GpsMeasurement_t>(const std::vector<std::string> &line) -> GpsMeasurement_t;
}

#endif //FLIGHTCOMPUTER_GPS_MEASUREMENT_T_HPP
