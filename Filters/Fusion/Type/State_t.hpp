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
        uint8_t bnoState, bnoError;
        uint8_t sysCalibStatus, gyroCalibStatus, accCalibStatus, magCalibStatus;
        si::default_type roll, pitch, yaw;
        si::Hertz<> rollDeriv, pitchDeriv, yawDeriv;
        si::Acceleration<> accX, accY, accZ;
        si::default_type elevonLeft, elevonRight;
        si::default_type motor;
};

class PdbPackage {
    public:
        si::Volt<> voltageVcc;
        si::Ampere<> currentVcc;
        si::Volt<> voltage5V;
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
        FlightMode flightMode = FlightMode::RTH;
        bool isArmed;
        int rssi;
};

class NavPackage {
    public:
        int rssi;
        si::Meter<> baroAltitude;
        si::Volt<> pitotVoltage;
        si::Meter<> usDistance;
};


class State_t {
    public:
        si::default_type roll{};
        si::Hertz<> rollDeriv{};
        si::default_type pitch{};
        si::Hertz<> pitchDeriv{};
        si::default_type yaw{};
        si::Hertz<> yawDeriv{};
        si::Speed<> speed{};
        si::Meter<> altitudeAboveGround{};
        si::Meter<> altitudeGround{};
        Gps_t position{0,0};
        si::Acceleration<> accX{}, accY{}, accZ{};
        Gps_t startLocation{0,0,0 * si::meter};
        si::Second<long double> startTime{};

        FlightControllerPackage rawFlightControllerData{};
        PdbPackage pdbPackage{};
        TaranisPackage taranisPackage{};
        LoraPackage loraRemote{};
        NavPackage navPackage{};

        friend std::ostream &operator<<(std::ostream &ostream, State_t state) {
            ostream << "(" << state.roll << "," << state.pitch << "," << state.yaw << ")";
            ostream << "\tGnd:" << state.altitudeAboveGround;
            ostream << " Sea:" << state.position.altitude;
            ostream << "\t" << state.speed;
            ostream << "\tAcc:(" << state.accX << "," << state.accY << "," << state.accZ << ")";
            ostream << "\tPos:(" << state.position.lat << "," << state.position.lon << ")";
            return ostream;
        }
};


#endif //FLIGHTCOMPUTER_STATE_T_HPP
