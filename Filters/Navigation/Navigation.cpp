//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"

void Navigation::run() {
    while(!in.isClosed() && !out.isClosed()) {
        State_t currentState{};
        Nav_t nav{};
        if(in.get(currentState)) {
            nav.heading = currentState.position.angleTo(Gps_t{0});
            double heightDifference = currentState.heightAboveGround - 50;
            nav.pitch = heightDifference;

            double speedDifference = currentState.airspeed - 50;
            double speed = speedDifference / 20.0 + nav.pitch / 30.0;

            if(speed < 0.0) {
                speed = 0;
            } else if(speed > 1.0) {
                speed = 1.0;
            }
            nav.power = speed;

            out.put(nav);
        }
    }
}

Channel<Nav_t> &Navigation::getChannelOut() {
    return out;
}

Channel<State_t> &Navigation::getChannelIn() {
    return in;
}
