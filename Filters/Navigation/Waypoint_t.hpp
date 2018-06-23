//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_WAYPOINT_T_HPP
#define FLIGHTCOMPUTER_WAYPOINT_T_HPP

#include "../../Devices/GPS/Gps_t.hpp"

class Waypoint_t {
public:
    Waypoint_t() : location(0,0), maxDelta(5), landingAllowed(false) {};
    Waypoint_t(double lat, double lon, double altitude, double maxDelta = 5, bool landingAllowed = false) :
            location(lat, lon, altitude), maxDelta(maxDelta), landingAllowed(landingAllowed){};
    Waypoint_t(const std::vector<std::string> &items) :
        location(std::stod(items[0]),std::stod(items[1]),std::stod(items[2])),
        maxDelta(std::stod(items[3])), landingAllowed(static_cast<bool>(std::stoi(items[4]))) {}

    Gps_t location;
    double maxDelta;
    bool landingAllowed;
};


#endif //FLIGHTCOMPUTER_WAYPOINT_T_HPP
