//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_STATE_T_HPP
#define FLIGHTCOMPUTER_STATE_T_HPP

#include <ostream>
#include "../../../Devices/GPS/Type/GpsMeasurement_t.hpp"
#include "../../../Utilities/Si/SiExtended.hpp"

enum class FlightMode {
    RTH = 0, LAUNCH = 1, LAND = 2, LOITER = 3, WAYPOINT = 4
};

enum class SwitchPos {
    UP, CENTRE, DOWN
};

class FlightControllerPackage {
    public:
        uint8_t bnoState, bnoError, calibStat;
        si::default_type roll, pitch, yaw;
        si::Frequency<> rollDeriv, pitchDeriv, yawDeriv;
        si::Acceleration<> accX, accY, accZ;
        si::default_type elevonLeft, elevonRight;
        si::default_type motor;
};

class PdbPackage {
    public:
        si::Voltage<> voltageVcc;
        si::Ampere<> currentVcc;
        si::Voltage<> voltage5V;
        si::Ampere<> current5V;
};

class TaranisPackage {
    public:
        si::default_type throttle, pitch, roll;
        bool isArmed, manualOverrideActive;
        si::default_type rssi;
};

class LoraPackage {
    public:
        si::default_type joyLeftX, joyRightX, joyLeftY, joyRightY;
        FlightMode flightMode = FlightMode::WAYPOINT;
        bool isArmed;
        int rssi;
};

class NavPackage {
    public:
        int rssi;
        si::Meter<> baroAltitude;
        si::Voltage<> pitotVoltage;
        si::Meter<> usDistance;
};


class State_t {
    public:
        si::default_type roll{};
        si::default_type pitch{};
        si::default_type yaw{};
        si::Speed<> speed{};
        si::Meter<> altitude{};
        si::Meter<> altitudeAboveGround{};
        si::Meter<> altitudeGround;
        double lat{}, lon{};
        si::Acceleration<> accX, accY, accZ;
        Gps_t startLocation{0,0,0 * si::meter};
        si::Second<long double> startTime;

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
            return ostream;
        }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
