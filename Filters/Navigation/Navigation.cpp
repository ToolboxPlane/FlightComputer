//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"
#include "../Fusion/State_t.hpp"
#include <limits>

namespace filter {
    Navigation::Navigation() {
        this->start();
    }

    OutputChannel<Nav_t> &Navigation::getChannelOut() {
        return out;
    }

    InputChannel<State_t> &Navigation::getChannelStateIn() {
        return stateIn;
    }

    InputChannel<Waypoint_t> &Navigation::getChannelWaypointIn() {
        return waypointIn;
    }

    void Navigation::run() {
        State_t currentState{};
        static FlightMode lastFLightMode;

        while (!stateIn.isClosed()) {
            if (stateIn.get(currentState)) {
                switch (currentState.lora.flightMode) {
                    case FlightMode::LAUNCH:
                        launch(currentState, lastFLightMode != currentState.lora.flightMode);
                        break;
                    case FlightMode::LAND:
                        land(currentState, lastFLightMode != currentState.lora.flightMode);
                        break;
                    case FlightMode::ANGLE:
                        angle(currentState, lastFLightMode != currentState.lora.flightMode);
                        break;
                    case FlightMode::HOLD:
                        hold(currentState, lastFLightMode != currentState.lora.flightMode);
                        break;
                    case FlightMode::WAYPOINT:
                        waypoints(currentState, lastFLightMode != currentState.lora.flightMode);
                        break;
                }
                lastFLightMode = currentState.lora.flightMode;
            }
        }
    }

    void Navigation::waypoints(State_t currentState, bool) {
        static Waypoint_t nextWaypoint(currentState.position.location, std::numeric_limits<double>::max(), false);
        static uint16_t waypointIndex = 0;
        Nav_t nav{};

        if (currentState.position.location.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
            waypointIn.get(nextWaypoint);
            waypointIndex++;
        }

        nav.heading = currentState.position.location.angleTo(nextWaypoint.location);
        nav.altitude = nextWaypoint.location.altitude;

        nav.speed = currentState.lora.isArmed ? CRUISE_SPEED : 0;

        nav.stateMajor = 4;
        nav.stateMinor = waypointIndex;

        out.put(nav);
    }

    void Navigation::land(State_t state, bool reset) {
        static double targetHeading = state.heading;
        static double targetAltitude = state.heightAboveSeaLevel;
        if (reset) {
            targetHeading = state.heading;
            targetAltitude = state.heightAboveSeaLevel;
        }

        Nav_t nav{};
        nav.speed = 0;
        nav.heading = targetHeading;
        nav.altitude = targetAltitude;

        nav.stateMajor = 2;
        nav.stateMinor = 0;

        out.put(nav);
    }

    void Navigation::launch(State_t state, bool reset) {
        static enum {
            IN_HAND, THROWN, CLIMB
        } launchState = IN_HAND;

        if (reset) {
            launchState = IN_HAND;
        }
        static double headingTarget, altitudeTarget;

        Nav_t nav{};

        switch (launchState) {
            case IN_HAND:
                nav.speed = 0.0;
                nav.heading = state.heading;
                nav.altitude = state.heightAboveSeaLevel;
                if (state.accForward > THROW_THRESH) {
                    headingTarget = state.heading;
                    altitudeTarget = state.heightAboveSeaLevel + POST_LAUNCH_ALTITUDE;
                    launchState = THROWN;
                }
                break;
            case THROWN:
                nav.speed = state.lora.isArmed ? std::numeric_limits<double>::max() : 0.0;
                nav.altitude = altitudeTarget;
                nav.heading = headingTarget;
                if (state.airspeed >= CRUISE_SPEED) {
                    launchState = CLIMB;
                }
                break;
            case CLIMB:
                nav.speed = state.lora.isArmed ? CRUISE_SPEED : 0;
                nav.altitude = altitudeTarget;
                nav.heading = headingTarget;
                break;
        }

        nav.stateMajor = 1;
        nav.stateMinor = launchState;

        out.put(nav);
    }

    void Navigation::angle(State_t , bool ) {
        /*Nav_t nav{};
        nav.pitch = state.lora.joyRight.y * MAX_PITCH;
        nav.roll = state.lora.joyRight.x * MAX_ROLL;
        nav.power = state.lora.isArmed ? speedControl(state.airspeed) : 0;

        nav.stateMajor = 0;
        nav.stateMinor = 0;

        out.put(nav);*/
        //@TODO
    }

    void Navigation::hold(State_t state, bool reset) {
        Nav_t nav{};
        static double targetHeading = state.heading;
        static double targetAltitude = state.heightAboveSeaLevel;
        if (reset) {
            targetHeading = state.heading;
            targetAltitude = state.heightAboveSeaLevel;
        }

        nav.speed = state.lora.isArmed ? CRUISE_SPEED : 0;
        nav.heading = targetHeading;
        nav.altitude = targetAltitude;

        nav.stateMajor = 3;
        nav.stateMinor = 0;

        out.put(nav);
    }

}
