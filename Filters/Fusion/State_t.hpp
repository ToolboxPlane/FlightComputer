//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_STATE_T_HPP
#define FLIGHTCOMPUTER_STATE_T_HPP


#include <ostream>

class State_t {
public:
    Gps_t position;
    double heightAboveGround, heightAboveSeaLevel;
    double pitch, roll, heading;
    double airspeed, groundSpeed;
    int flightmode, armed;

    friend std::ostream &operator<<(std::ostream &ostream, State_t state) {
        ostream << "Heading: " << state.heading;
        ostream << "\tRoll: " << state.roll;
        ostream << "\tPitch: " << state.pitch;
        ostream << "\tHeight above ground: " << state.heightAboveGround;
        ostream << "\tHeight above sea: " << state.heightAboveSeaLevel;

        return ostream;
    }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
