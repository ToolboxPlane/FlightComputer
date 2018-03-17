//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"

void Navigation::run() {
    while(!in.isClosed() && !out.isClosed()) {
        state_t currentState{};
        nav_t nav{};
        if(in.get(currentState)) {
            nav.pitch = 0;
            nav.heading = 0;
            nav.power = 1.0;
            out.put(nav);
        }
    }
}

Channel<nav_t> &Navigation::getChannelOut() {
    return out;
}

Channel<state_t> &Navigation::getChannelIn() {
    return in;
}
