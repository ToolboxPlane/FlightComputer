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

    void Navigation::waypoints(State_t currentState, bool reset) {
        static Waypoint_t nextWaypoint(currentState.position.location, std::numeric_limits<double>::max(), false);
        static uint16_t waypointIndex = 0;
        Nav_t nav{};

        if (currentState.position.location.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
            waypointIn.get(nextWaypoint);
            waypointIndex++;
        }

        double headingTarget = currentState.position.location.angleTo(nextWaypoint.location);
        nav.roll = headingControl(currentState.heading, headingTarget);

        double deltaHeight = currentState.heightAboveSeaLevel - nextWaypoint.location.altitude;
        nav.pitch = deltaHeight * Navigation::PITCH_P;

        if (nav.pitch < -MAX_PITCH) {
            nav.pitch = -MAX_PITCH;
        } else if (nav.pitch > MAX_PITCH) {
            nav.pitch = MAX_PITCH;
        }

        nav.power = currentState.lora.isArmed ? speedControl(currentState.airspeed) : 0;

        nav.stateMajor = 4;
        nav.stateMinor = waypointIndex;

        out.put(nav);
    }

    void Navigation::land(State_t state, bool reset) {
        static double targetHeading = state.heading;
        if (reset) {
            targetHeading = state.heading;
        }

        Nav_t nav{};
        nav.power = 0;
        nav.roll = headingControl(state.heading, targetHeading);
        nav.pitch = 0;

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

        Nav_t nav{};

        switch (launchState) {
            case IN_HAND:
                nav.power = 0.0;
                nav.roll = 0.0;
                nav.pitch = 0.0;
                if (state.accForward > THROW_THRESH) {
                    launchState = THROWN;
                }
                break;
            case THROWN:
                nav.power = state.lora.isArmed ? 1.0 : 0.0;
                nav.roll = 0.0;
                nav.pitch = 0.0;
                if (state.airspeed >= CRUISE_SPEED) {
                    launchState = CLIMB;
                }
                break;
            case CLIMB:
                nav.power = state.lora.isArmed ? speedControl(state.airspeed) : 0;
                nav.pitch = POST_LAUNCH_CLIMB;
                nav.roll = 0;
                break;
        }

        nav.stateMajor = 1;
        nav.stateMinor = launchState;

        out.put(nav);
    }

    void Navigation::angle(State_t state, bool reset) {
        Nav_t nav{};
        nav.pitch = state.lora.joyRight.y * MAX_PITCH;
        nav.roll = state.lora.joyRight.x * MAX_ROLL;
        nav.power = state.lora.isArmed ? speedControl(state.airspeed) : 0;

        nav.stateMajor = 0;
        nav.stateMinor = 0;

        out.put(nav);
    }

    void Navigation::hold(State_t state, bool reset) {
        Nav_t nav{};
        static double targetHeading = state.heading;
        if (reset) {
            targetHeading = state.heading;
        }

        nav.power = state.lora.isArmed ? speedControl(state.airspeed) : 0;
        nav.pitch = 0;
        nav.roll = headingControl(state.heading, targetHeading);

        nav.stateMajor = 3;
        nav.stateMinor = 0;

        out.put(nav);
    }

    auto Navigation::speedControl(double airspeed, double target) -> double {
        static double diffSum = 0;
        static double lastDiff = target - airspeed;
        double deltaSpeed = target - airspeed;
        diffSum += deltaSpeed;

        // Change in sign, anti windup
        if (lastDiff * deltaSpeed < 0) {
            diffSum = 0;
        }
        lastDiff = deltaSpeed;

        // PI-Controller (I necessary to achieve stationary accuracy)
        double speed = deltaSpeed * Navigation::SPEED_P + diffSum * Navigation::SPEED_I;

        if (speed < 0.0) {
            speed = 0;
        } else if (speed > 1.0) {
            speed = 1.0;
        }
        return speed;
    }

    auto Navigation::headingControl(double currHeading, double target) -> double {
        double headingDiff = target - currHeading;
        headingDiff = fmod(headingDiff, 360);
        if (headingDiff > 180) {
            headingDiff -= 180;
        } else if (headingDiff < -180) {
            headingDiff += 360;
        }
        double roll = -headingDiff * HEADING_P; // sign is a result of compass angles not being mathematically positive
        if (roll < -MAX_ROLL) {
            roll = -MAX_ROLL;
        } else if (roll > MAX_ROLL) {
            roll = MAX_ROLL;
        }
        return roll;
    }
}
