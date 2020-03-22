//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"
#include "../../Fusion/Type/State_t.hpp"
#include <limits>

namespace filter {
    using namespace si::base;
    using namespace si::literals;
    using namespace si::extended;

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
        static FlightMode lastFlightMode;

        while (!stateIn.isClosed()) {
            if (stateIn.get(currentState)) {
                switch (currentState.loraRemote.flightMode) {
                    case FlightMode::LAUNCH:
                        launch(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::LAND:
                        land(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::ANGLE:
                        angle(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::HOLD:
                        hold(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::WAYPOINT:
                        waypoints(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    default:
                        std::cerr << "FlightMode not valid!" << std::endl;
                }
                lastFlightMode = currentState.loraRemote.flightMode;
            }
        }
    }

    void Navigation::waypoints(State_t currentState, bool) {
        static Waypoint_t nextWaypoint({currentState.lat, currentState.lon}, std::numeric_limits<double>::max(), false);
        static uint16_t waypointIndex = 0;
        Nav_t nav{};

        Gps_t position{currentState.lat, currentState.lon};

        if (position.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
            waypointIn.get(nextWaypoint);
            waypointIndex++;
        }

        nav.heading = position.angleTo(nextWaypoint.location);
        nav.altitude = nextWaypoint.location.altitude;

        nav.speed = CRUISE_SPEED;

        nav.stateMajor = 4;
        nav.stateMinor = waypointIndex;

        out.put(nav);
    }

    void Navigation::land(State_t state, bool reset) {
        static auto targetHeading = state.yaw;
        static auto targetAltitude = state.altitude;
        if (reset) {
            targetHeading = state.yaw;
            targetAltitude = state.altitude;
        }

        Nav_t nav{};
        nav.speed = 0_speed;
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
        static double headingTarget;
        static si::base::Meter<> altitudeTarget;

        Nav_t nav{};

        switch (launchState) {
            case IN_HAND:
                nav.speed = 0.0_speed;
                nav.heading = state.yaw;
                nav.altitude = state.altitude;
                if (THROW_THRESH < 0 * si::extended::acceleration) { //@TODO fixme
                    headingTarget = state.yaw;
                    altitudeTarget = state.altitude + POST_LAUNCH_ALTITUDE;
                    launchState = THROWN;
                }
                break;
            case THROWN:
                nav.speed = std::numeric_limits<double>::max() * si::extended::speed;
                nav.altitude = altitudeTarget;
                nav.heading = headingTarget;
                if (CRUISE_SPEED < state.speed) {
                    launchState = CLIMB;
                }
                break;
            case CLIMB:
                nav.speed = CRUISE_SPEED;
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
        nav.pitch = state.loraRemote.joyRight.y * MAX_PITCH;
        nav.roll = state.loraRemote.joyRight.x * MAX_ROLL;
        nav.power = state.loraRemote.isArmed ? speedControl(state.airspeed) : 0;

        nav.stateMajor = 0;
        nav.stateMinor = 0;

        out.put(nav);*/
        //@TODO
    }

    void Navigation::hold(State_t state, bool reset) {
        Nav_t nav{};
        static auto targetHeading = state.yaw;
        static auto targetAltitude = state.altitude;
        if (reset) {
            targetHeading = state.yaw;
            targetAltitude = state.altitude;
        }

        nav.speed = CRUISE_SPEED;
        nav.heading = targetHeading;
        nav.altitude = targetAltitude;

        nav.stateMajor = 3;
        nav.stateMinor = 0;

        out.put(nav);
    }

}
