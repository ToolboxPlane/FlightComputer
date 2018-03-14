//
// Created by paul on 14.03.18.
//

#include <iostream>
#include "FusionDebug.hpp"

Channel<state_t> &FusionDebug::getChannelIn() {
    return in;
}

void FusionDebug::run() {
    while(!in.isClosed()) {
        state_t state{};
        if(in.get(state)) {
            std::cout << "[" << tag << "]\t";
            std::cout << "Heading: " << state.heading;
            std::cout << "\tRoll: " << state.roll;
            std::cout << "\tPitch: " << state.pitch;
            std::cout << "\tHeight above ground: " << state.heightAboveGround;
            std::cout << "\tHeight above sea: " << state.heightAboveSeaLevel;
            std::cout << std::endl;
        }
    }
}
