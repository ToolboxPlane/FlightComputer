/**
 * @file Waypoint_t.cpp
 * @author paul
 * @date 04.04.20
 * Description here TODO
 */

#include "Waypoint_t.hpp"

#include <cassert>

namespace recording {
    template<>
    auto getItem(const std::vector<std::string> &line) -> Waypoint_t {
        assert(line.size() == 5);
        Waypoint_t res;
        res.location.lat = std::stod(line[0]);
        res.location.lon = std::stod(line[1]);
        res.location.altitude = std::stof(line[2]) * si::meter;
        res.maxDelta = std::stof(line[3]) * si::meter;
        res.landingAllowed = static_cast<bool>(std::stoi(line[4]));

        return res;
    }
} // namespace recording
