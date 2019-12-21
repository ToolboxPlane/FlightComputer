//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_STATE_T_HPP
#define FLIGHTCOMPUTER_STATE_T_HPP

#include <ostream>
#include "../../../Devices/GPS/Type/GpsMeasurement_t.hpp"
#include "../../../Utilities/Si/SiExtended.hpp"

enum class FlightMode{
    ANGLE = 0, LAUNCH = 1, LAND = 2, HOLD = 3, WAYPOINT = 4
};

class FlightControllerPackage {
public:
    uint8_t bnoState;
    float roll, pitch, yaw;
    float rollDeriv, pitchDeriv, yawDeriv;
    float aileronRight, vtailRight;
    float aileronLeft, vtailLeft;
    float motor;
};

class State_t {
public:
    GpsMeasurement_t position;
    si::base::MeterType<> heightAboveGround, heightAboveSeaLevel;
    double pitch, roll, yaw;
    double pitchDeriv, rollDeriv, yawDeriv;
    si::extended::SpeedType<> airspeed, groundSpeed;
    si::extended::VoltageType<> voltage;

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
        ostream << "H:" << state.yaw;
        ostream << "\tR:" << state.roll;
        ostream << "\tP:" << state.pitch;
        ostream << "\tGnd:" << state.heightAboveGround;
        ostream << "\tSea:" << state.heightAboveSeaLevel;
        ostream << "\tAirs:" << state.airspeed;
        ostream << "\tGnds:" << state.groundSpeed;
        if(state.position.fixAquired) {
            ostream << "\tPos:(" << state.position.location.lat << "," << state.position.location.lon << ")";
        } else {
            ostream << "\t No Fix";
        }
        ostream << "\tV:" << state.voltage;
        ostream << "\tAr:" << (state.taranis.isArmed?1:0);
        ostream << "\tOr:" << (state.taranis.manualOverrideActive?1:0);
        return ostream;
    }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
