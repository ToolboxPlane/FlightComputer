/**
 * @file GpsMeasurement_t.cpp
 * @author paul
 * @date 03.04.20
 * Description here TODO
 */

#include "GpsMeasurement_t.hpp"

#include <cassert>

namespace recording {
    template<>
    auto header<GpsMeasurement_t>() -> std::string {
        return "Timestamp; Fix; Lat; Lon; Altitude; Speed; Climb; EpLat; EpLon; EpVert; EpSpeed; EpClimb";
    }

    template<>
    auto getLine<GpsMeasurement_t>(const GpsMeasurement_t &gpsMeasurement) -> std::string {
        std::stringstream stringstream;
        stringstream << static_cast<long double>(gpsMeasurement.timestamp) << "; ";
        stringstream << (gpsMeasurement.fixAquired ? 1 : 0) << "; ";
        stringstream << gpsMeasurement.location.lat << "; ";
        stringstream << gpsMeasurement.location.lon << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.location.altitude) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.speed) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.climb) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.epLat) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.epLon) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.epVert) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.epSpeed) << "; ";
        stringstream << static_cast<si::default_type>(gpsMeasurement.epClimb);
        return stringstream.str();
    }

    template<>
    auto getItem<GpsMeasurement_t>(const std::vector<std::string> &line) -> GpsMeasurement_t {
        assert(line.size() == 12);

        GpsMeasurement_t measurement;
        measurement.timestamp = si::Second<long double>(std::stold(line[0]));
        measurement.fixAquired = static_cast<bool>(std::stoi(line[1]));
        measurement.location.lat = std::stod(line[2]);
        measurement.location.lon = std::stod(line[3]);
        measurement.location.altitude = std::stof(line[4]) * si::meter;
        measurement.speed = std::stof(line[5]) * si::speed;
        measurement.climb = std::stof(line[6]) * si::speed;
        measurement.epLat = std::stof(line[7]) * si::meter;
        measurement.epLon = std::stof(line[8]) * si::meter;
        measurement.epVert = std::stof(line[9]) * si::meter;
        measurement.epSpeed = std::stof(line[10]) * si::speed;
        measurement.epClimb = std::stof(line[11]) * si::speed;

        return measurement;
    }
} // namespace recording
