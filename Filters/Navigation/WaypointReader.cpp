//
// Created by paul on 14.05.18.
//

#include "WaypointReader.hpp"

WaypointReader::WaypointReader(std::istream &istream) : istream(istream){
    this->start();
}

MultipleOutputChannel<Waypoint_t> &WaypointReader::getChannelOut() {
    return out;
}

void WaypointReader::run() {
    Waypoint_t waypoint;
    json waypointList;
    istream >> waypointList;
    for(const auto &waypointItem: waypointList) {
        waypoint.location = Gps_t(waypointItem["lat"], waypointItem["lon"], waypointItem["altitude"]);
        waypoint.landingAllowed = waypointItem["landingAllowed"];
        waypoint.maxDelta = waypointItem["maxDelta"];
        out.put(waypoint);
    }
}

