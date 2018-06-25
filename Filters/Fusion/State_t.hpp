//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_STATE_T_HPP
#define FLIGHTCOMPUTER_STATE_T_HPP

#include <ostream>

enum class FlightMode{
    LAUNCH, LAND, ANGLE, HOLD, WAYPOINT
};

class State_t {
public:
    GpsMeasurement_t position;
    double heightAboveGround, heightAboveSeaLevel;
    double pitch, roll, heading;
    double airspeed, groundSpeed;
    double voltage;
    double accForward, accSide, accUpdown;

    struct {
        int throttle, yaw, pitch, roll;
        bool isArmed, manualOverrideActive;
    } taranis;

    struct {
        struct {
            double x,y;
        }joyLeft, joyRight;

        bool isArmed;
        FlightMode flightMode;
    } lora;

    friend std::ostream &operator<<(std::ostream &ostream, State_t state) {
        ostream << "H:" << state.heading;
        ostream << "\tR:" << state.roll;
        ostream << "\tP:" << state.pitch;
        ostream << "\tGnd:" << state.heightAboveGround;
        ostream << "\tSea:" << state.heightAboveSeaLevel;
        ostream << "\tAirs:" << state.airspeed;
        ostream << "\tGnds:" << state.groundSpeed;
        ostream << "\tPos:(" << state.position.location.lat << "," << state.position.location.lon << ")";
        ostream << "\tV:" << state.voltage;
        ostream << "\tAr:" << state.taranis.isArmed?1:0;
        ostream << "\tOr:" << state.taranis.manualOverrideActive?1:0;
        return ostream;
    }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
