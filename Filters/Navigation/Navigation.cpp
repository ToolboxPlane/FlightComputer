//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"

void Navigation::run() {
    while(!stateIn.isClosed() && !out.isClosed()) {
        State_t currentState{};
        Nav_t nav{};
        Waypoint_t nextWaypoint;
        waypointIn.get(nextWaypoint);

        if(stateIn.get(currentState)) {
            if(currentState.position.location.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
                waypointIn.get(nextWaypoint);
            }
            nav.heading = currentState.position.location.distanceTo(nextWaypoint.location);

            double heightDifference = currentState.heightAboveGround - 50;
            nav.pitch = heightDifference;

            if(nav.pitch < -30) {
                nav.pitch = 30;
            } else if(nav.pitch > 30) {
                nav.pitch = 30;
            }

            double speedDifference = currentState.airspeed - 50;
            double speed = speedDifference / 20.0 + nav.pitch / 30.0;

            if(speed < 0.0) {
                speed = 0;
            } else if(speed > 1.0) {
                speed = 1.0;
            }


            out.put(nav);
        }
    }
}

Channel<Nav_t> &Navigation::getChannelOut() {
    return out;
}

Channel<State_t> &Navigation::getChannelStateIn() {
    return stateIn;
}

Channel<Waypoint_t> &Navigation::getChannelWaypointIn() {
    return waypointIn;
}
