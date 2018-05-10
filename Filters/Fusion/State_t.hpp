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
        ostream << "Head: " << state.heading;
        ostream << "\tRoll: " << state.roll;
        ostream << "\tPitch: " << state.pitch;
        ostream << "\tHeight ground: " << state.heightAboveGround;
        ostream << "\tHeight sea: " << state.heightAboveSeaLevel;
        ostream << "\tAirspeed: " << state.airspeed;
        ostream << "\tGndspeed: " << state.groundSpeed;
        ostream << "\tPos: (" << state.position.lat << "," << state.position.lon << ")";
        return ostream;
    }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
