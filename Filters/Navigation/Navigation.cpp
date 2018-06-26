//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"
#include "../Fusion/State_t.hpp"
#include <limits>

Navigation::Navigation() {
    this->start();
}

MultipleOutputChannel<Nav_t> &Navigation::getChannelOut() {
    return out;
}

Channel<State_t> &Navigation::getChannelStateIn() {
    return stateIn;
}

Channel<Waypoint_t> &Navigation::getChannelWaypointIn() {
    return waypointIn;
}

void Navigation::run() {
    State_t currentState{};
    static FlightMode lastFLightMode;

    while(!stateIn.isClosed()) {
        if(stateIn.get(currentState)) {
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
    Nav_t nav{};

    if(currentState.position.location.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
        waypointIn.get(nextWaypoint);
    }

    double headingTarget = currentState.position.location.angleTo(nextWaypoint.location);
    double headingDiff = headingTarget - currentState.heading;
    headingDiff = fmod(headingDiff, 360);
    if(headingDiff > 180) {
        headingDiff -= 180;
    }
    nav.roll = headingDiff * HEADING_P;
    if(nav.roll < MAX_ROLL) {
        nav.roll = -MAX_ROLL;
    } else if(nav.pitch > MAX_ROLL) {
        nav.pitch = MAX_ROLL;
    }

    nav.roll = currentState.position.location.angleTo(nextWaypoint.location);

    double deltaHeight = currentState.heightAboveSeaLevel - nextWaypoint.location.altitude;
    nav.pitch = deltaHeight * Navigation::PITCH_P;

    if(nav.pitch < -MAX_PITCH) {
        nav.pitch = MAX_PITCH;
    } else if(nav.pitch > MAX_PITCH) {
        nav.pitch = MAX_PITCH;
    }

    nav.power = speedControl(currentState.airspeed);

    out.put(nav);
}

void Navigation::land(State_t state, bool reset) {
    Nav_t nav{};
    nav.power = 0;
    nav.roll = 0;
    nav.pitch = 0;

    out.put(nav);
}

void Navigation::launch(State_t state, bool reset) {
    static enum {
        IN_HAND, THROWN, CLIMB
    } launchState = IN_HAND;

    if(reset) {
        launchState = IN_HAND;
    }

    Nav_t nav{};

    switch (launchState) {
        case IN_HAND:
            nav.power = 0.0;
            nav.roll = 0.0;
            nav.pitch = 0.0;
            if(state.accForward > THROW_THRESH) {
                launchState = THROWN;
            }
            break;
        case THROWN:
            nav.power = 1.0;
            nav.roll = 0.0;
            nav.pitch = 0.0;
            if(state.airspeed >= CRUISE_SPEED) {
                launchState = CLIMB;
            }
            break;
        case CLIMB:
            nav.power = speedControl(state.airspeed);
            nav.pitch = POST_LAUNCH_CLIMB;
            nav.roll = 0;
            break;
    }
    out.put(nav);
}

void Navigation::angle(State_t state, bool reset) {
    Nav_t nav{};
    nav.pitch = state.lora.joyRight.y * MAX_PITCH;
    nav.roll = state.lora.joyRight.x * MAX_ROLL;
    nav.power = speedControl(state.airspeed);

    out.put(nav);
}

void Navigation::hold(State_t state, bool reset) {
    Nav_t nav{};
    nav.power = speedControl(state.airspeed);
    nav.pitch = 0;
    nav.roll = 0;

    out.put(nav);
}

auto Navigation::speedControl(double airspeed, double target) -> double {
    static double diffSum = 0;
    double deltaSpeed = airspeed - target;
    diffSum += deltaSpeed;
    double speed = deltaSpeed * Navigation::SPEED_P + diffSum * Navigation::SPEED_I;

    if(speed < 0.0) {
        speed = 0;
    } else if(speed > 1.0) {
        speed = 1.0;
    }
    return speed;
}

