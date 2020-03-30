//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_STATE_T_HPP
#define FLIGHTCOMPUTER_STATE_T_HPP

#include <ostream>
#include "../../../Devices/GPS/Type/GpsMeasurement_t.hpp"
#include "../../../Utilities/Si/SiExtended.hpp"

enum class FlightMode {
    ANGLE = 0, LAUNCH = 1, LAND = 2, HOLD = 3, WAYPOINT = 4
};

enum class SwitchPos {
    UP, CENTRE, DOWN
};

class FlightControllerPackage {
public:
    uint8_t bnoState;
    float roll, pitch, yaw;
    si::extended::Frequency<> rollDeriv, pitchDeriv, yawDeriv;
    si::extended::Acceleration<> accX, accY, accZ;
    float elevonLeft, elevonRight;
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
    float throttle, pitch, roll;
    bool isArmed, manualOverrideActive;
    float rssi;
};

class LoraPackage {
public:
    float joyLeftX, joyRightX, joyLeftY, joyRightY;
    FlightMode flightMode = FlightMode::WAYPOINT;
    bool isArmed;
};

class NavPackage {
    public:
        int rssi;
        si::base::Meter<> baroAltitude;
        si::extended::Voltage<> pitotVoltage;
        si::base::Meter<> usDistance;
};


class State_t {
public:
    float roll{};
    float pitch{};
    float yaw{};
    si::extended::Speed<> speed{};
    si::base::Meter<> altitude{};
    si::base::Meter<> altitudeAboveGround{};
    si::base::Meter<> altitudeGround;
    float lat{}, lon{};
    si::extended::Acceleration<> accX, accY, accZ;

    FlightControllerPackage rawFlightControllerData{};
    PdbPackage pdbPackage{};
    TaranisPackage taranisPackage{};
    LoraPackage loraRemote{};
    NavPackage navPackage;

    friend std::ostream &operator<<(std::ostream &ostream, State_t state) {
        ostream << "H:" << state.yaw;
        ostream << "\tR:" << state.roll;
        ostream << "\tP:" << state.pitch;
        ostream << "\tGnd:" << state.altitudeAboveGround;
        ostream << "\tSea:" << state.altitude;
        ostream << "\tSpeed:" << state.speed;
        ostream << "\tAcc:(" << state.accX << "," << state.accY << "," << state.accZ << ")";
        ostream << "\tPos:(" << state.lat << "," << state.lon << ")";
        ostream << "\tV:" << state.pdbPackage.voltageVcc;
        ostream << "\tAr:" << (state.taranisPackage.isArmed?1:0);
        ostream << "\tOr:" << (state.taranisPackage.manualOverrideActive?1:0);
        return ostream;
    }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
