//
// Created by paul on 14.05.18.
//

#ifndef FLIGHTCOMPUTER_WAYPOINT_T_HPP
#define FLIGHTCOMPUTER_WAYPOINT_T_HPP

#include <ostream>
#include "../../../Devices/GPS/Type/Gps_t.hpp"
#include "../../../Utilities/Recording/Serialization.hpp"

class Waypoint_t {
    public:
        Waypoint_t() : location(0, 0), maxDelta(5), landingAllowed(false) {};

        Waypoint_t(double lat, double lon, si::Meter<> altitude, si::default_type maxDelta = 5,
                   bool landingAllowed = false) :
                location(lat, lon, altitude), maxDelta(maxDelta), landingAllowed(landingAllowed) {};

        Waypoint_t(const std::vector<std::string> &items) :
                location(std::stod(items[0]), std::stod(items[1]), std::stof(items[2]) * si::meter),
                maxDelta(std::stof(items[3])), landingAllowed(static_cast<bool>(std::stoi(items[4]))) {};

        Waypoint_t(Gps_t gps, double maxDelta, bool landingAllowed) : location(gps), maxDelta(maxDelta),
                                                                      landingAllowed(landingAllowed) {};

        Gps_t location;
        si::Meter<> maxDelta;
        bool landingAllowed;

        friend std::ostream &operator<<(std::ostream &ostream, const Waypoint_t &waypoint) {
            ostream << "(" << waypoint.location.lat << ", " << waypoint.location.lon << ")"
                    << "\tdelta: " << waypoint.maxDelta
                    << "\tlandingAllowed: " << waypoint.landingAllowed;
            return ostream;
        }
};

namespace recording {
     template<>
     auto getItem(const std::vector<std::string> &line) -> Waypoint_t;
}


#endif //FLIGHTCOMPUTER_WAYPOINT_T_HPP
