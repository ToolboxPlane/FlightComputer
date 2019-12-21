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
    si::extended::Frequency<> rollDeriv, pitchDeriv, yawDeriv;
    float aileronRight, vtailRight;
    float aileronLeft, vtailLeft;
    float motor;
};

class PdbPackage {
public:
    uint8_t status;
    si::extended::Voltage<> voltageVcc;
    si::base::Ampere<> currentVcc;
    si::extended::Voltage<> voltage5V;
    si::base::Ampere<> current5V;
};

class TaranisPackage {
public:
    float throttle, yaw, pitch, roll;
    bool isArmed, manualOverrideActive;
};

class LoraPackage {
public:
    float joyLeftX, joyRightX, joyLeftY, joyRightY;
    FlightMode flightMode;
    bool isArmed;
};


class State_t {
public:
    GpsMeasurement_t position;
    si::base::Meter<> heightAboveGround{}, heightAboveSeaLevel{};
    double pitch{}, roll{}, yaw{};
    si::extended::Speed<> airspeed{}, groundSpeed{};

    FlightControllerPackage rawFlightControllerData{};
    PdbPackage pdbPackage{};
    TaranisPackage taranisPackage{};
    LoraPackage loraRemote{};

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
        ostream << "\tV:" << state.pdbPackage.voltageVcc;
        ostream << "\tAr:" << (state.taranisPackage.isArmed?1:0);
        ostream << "\tOr:" << (state.taranisPackage.manualOverrideActive?1:0);
        return ostream;
    }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP