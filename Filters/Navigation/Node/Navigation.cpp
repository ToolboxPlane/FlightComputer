//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"
#include "../../Fusion/Type/State_t.hpp"
#include <limits>

namespace filter {
    using namespace si::literals;
    using namespace si;

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
        static auto lastFlightMode = FlightMode::RTH;

        while (!stateIn.isClosed()) {
            if (stateIn.get(currentState)) {
                switch (currentState.loraRemote.flightMode) {
                    case FlightMode::RTH:
                        rth(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::LAUNCH:
                        launch(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::LAND:
                        land(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::LOITER:
                        loiter(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    case FlightMode::WAYPOINT:
                        waypoint(currentState, lastFlightMode != currentState.loraRemote.flightMode);
                        break;
                    default:
                        std::cerr << "FlightMode not valid!" << std::endl;
                }
                lastFlightMode = currentState.loraRemote.flightMode;
            }
        }
    }

    void Navigation::rth(const State_t &currentState, bool) {
        Nav_t nav{};

        nav.heading = currentState.position.angleTo(currentState.startLocation);
        nav.altitude = currentState.startLocation.altitude + RTH_ALTITUDE;

        nav.speed = CRUISE_SPEED;

        nav.stateMajor = 0;
        nav.stateMinor = 0;
        nav.state = currentState;

        out.put(nav);
    }

    void Navigation::launch(const State_t &state, bool reset) {
        static enum {
            IN_HAND, THROWN, CLIMB
        } launchState = IN_HAND;

        if (reset) {
            launchState = IN_HAND;
        }
        static si::default_type headingTarget;
        static si::Meter<> altitudeTarget;

        Nav_t nav{};

        switch (launchState) {
            case IN_HAND:
                nav.speed = 0.0_speed;
                nav.heading = state.yaw;
                nav.altitude = state.position.altitude;
                if (LAUNCH_THROW_THRESH < state.accX && std::abs(state.roll) < LAUNCH_MAX_ROLL
                    && state.pitch > LAUNCH_MIN_PITCH && state.pitch < LAUNCH_MAX_PITCH) {
                    headingTarget = state.yaw;
                    altitudeTarget = state.position.altitude + LAUNCH_TARGET_ALTITUDE;
                    launchState = THROWN;
                }
                break;
            case THROWN:
                nav.speed = std::numeric_limits<si::default_type>::max() * si::speed;
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
        nav.state = state;

        out.put(nav);
    }

    void Navigation::land(const State_t &state, bool reset) {
        static auto targetHeading = state.yaw;
        if (reset) {
            targetHeading = state.yaw;
        }

        Nav_t nav{};
        nav.speed = 0_speed;
        nav.heading = targetHeading;
        if (LANDING_APPROACH_ALTITUDE + 0.1_meter < state.altitudeAboveGround) { // Approach
            nav.altitude = state.altitudeGround + LANDING_APPROACH_ALTITUDE;
            nav.stateMinor = 0;
        } else if (LANDING_SPEED < state.speed) { // Slow Down
            nav.altitude = state.altitudeAboveGround + LANDING_APPROACH_ALTITUDE;
            nav.stateMinor = 1;
        } else if (LANDING_FLARE_ALTITUDE < state.altitudeAboveGround) { // Landing
            nav.altitude = LANDING_FLARE_ALTITUDE - 0.1_meter;
            nav.stateMinor = 2;
        } else { // Flare
            nav.altitude = state.position.altitude + 100_meter;
            nav.stateMinor = 3;
        }

        nav.stateMajor = 2;
        nav.state = state;

        out.put(nav);
    }

    void Navigation::loiter(const State_t &state, bool reset) {
        Nav_t nav{};
        static auto centre = state.position;
        if (reset) {
            centre = state.position;
        }

        auto angleFromCentre = centre.angleTo(state.position);
        auto targetAngle = angleFromCentre + LOITER_TARGET_ANGLE;
        auto latDist = std::cos(targetAngle / 180 * M_PI_F) * LOITER_RADIUS;
        auto lonDist = std::sin(targetAngle / 180 * M_PI_F) * LOITER_RADIUS;
        Gps_t targetPosition{
            centre.lat + static_cast<double>(latDist / EARTH_CIRCUMFERENCE * 360),
            centre.lon + static_cast<double>(lonDist / EARTH_CIRCUMFERENCE * 360 / std::cos(centre.lat / 180 * M_PI_F))
        };

        nav.heading = state.position.angleTo(targetPosition);
        nav.speed = CRUISE_SPEED;
        nav.altitude = centre.altitude;

        nav.heading =

        nav.stateMajor = 3;
        nav.stateMinor = 0;
        nav.state = state;

        out.put(nav);
    }

    void Navigation::waypoint(const State_t &currentState, bool) {
        static Waypoint_t nextWaypoint(currentState.position, std::numeric_limits<si::default_type>::max(), false);
        static uint16_t waypointIndex = 0;
        Nav_t nav{};

        if (currentState.position.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
            waypointIn.get(nextWaypoint);
            waypointIndex++;
        }

        nav.heading = currentState.position.angleTo(nextWaypoint.location);
        nav.altitude = nextWaypoint.location.altitude;

        nav.speed = CRUISE_SPEED;

        nav.stateMajor = 4;
        nav.stateMinor = waypointIndex;
        nav.state = currentState;

        out.put(nav);
    }
}
