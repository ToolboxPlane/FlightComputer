//
// Created by paul on 14.03.18.
//

#include "Navigation.hpp"

#include <limits>

#include "../../Fusion/Type/State_t.hpp"

namespace filter {
    using namespace si::literals;
    using namespace si;

    Navigation::Navigation() {
        this->start();
    }

    OutputChannel<Nav> &Navigation::getChannelOut() {
        return out;
    }

    InputChannel<FusionResult> &Navigation::getChannelStateIn() {
        return stateIn;
    }

    InputChannel<Waypoint_t> &Navigation::getChannelWaypointIn() {
        return waypointIn;
    }

    void Navigation::run() {
        FusionResult fusionResult{};
        static auto lastFlightMode = FlightMode::RTH;

        while (!stateIn.isClosed()) {
            if (stateIn.get(fusionResult)) {
                switch (fusionResult.loraRemote.flightMode) {
                    case FlightMode::RTH:
                        rth(fusionResult, lastFlightMode != fusionResult.loraRemote.flightMode);
                        break;
                    case FlightMode::LAUNCH:
                        launch(fusionResult, lastFlightMode != fusionResult.loraRemote.flightMode);
                        break;
                    case FlightMode::LAND:
                        land(fusionResult, lastFlightMode != fusionResult.loraRemote.flightMode);
                        break;
                    case FlightMode::LOITER:
                        loiter(fusionResult, lastFlightMode != fusionResult.loraRemote.flightMode);
                        break;
                    case FlightMode::WAYPOINT:
                        waypoint(fusionResult, lastFlightMode != fusionResult.loraRemote.flightMode);
                        break;
                    default:
                        std::cerr << "FlightMode not valid!" << std::endl;
                }
                lastFlightMode = fusionResult.loraRemote.flightMode;
            }
        }
    }

    void Navigation::rth(const FusionResult &fusionResult, bool) {
        Nav nav{};

        nav.heading = fusionResult.state.position.angleTo(fusionResult.state.startLocation);
        nav.altitude = fusionResult.state.startLocation.altitude + RTH_ALTITUDE;

        nav.speed = CRUISE_SPEED;

        nav.stateMajor = 0;
        nav.stateMinor = 0;
        nav.fusionResult = fusionResult;

        out.put(nav);
    }

    void Navigation::launch(const FusionResult &fusionResult, bool reset) {
        static enum { IN_HAND, THROWN, CLIMB } launchState = IN_HAND;

        if (reset) {
            launchState = IN_HAND;
        }
        static si::default_type headingTarget;
        static si::Meter<> altitudeTarget;

        Nav nav{};

        switch (launchState) {
            case IN_HAND:
                nav.speed = 0.0_speed;
                nav.heading = fusionResult.state.yaw;
                nav.altitude = fusionResult.state.position.altitude;
                if (LAUNCH_THROW_THRESH < fusionResult.state.accX &&
                    std::abs(fusionResult.state.roll) < LAUNCH_MAX_ROLL &&
                    fusionResult.state.pitch > LAUNCH_MIN_PITCH && fusionResult.state.pitch < LAUNCH_MAX_PITCH) {
                    headingTarget = fusionResult.state.yaw;
                    altitudeTarget = fusionResult.state.position.altitude + LAUNCH_TARGET_ALTITUDE;
                    launchState = THROWN;
                }
                break;
            case THROWN:
                nav.speed = std::numeric_limits<si::default_type>::max() * si::speed;
                nav.altitude = altitudeTarget;
                nav.heading = headingTarget;
                if (CRUISE_SPEED < fusionResult.state.speed) {
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
        nav.fusionResult = fusionResult;

        out.put(nav);
    }

    void Navigation::land(const FusionResult &fusionResult, bool reset) {
        static auto targetHeading = fusionResult.state.yaw;
        if (reset) {
            targetHeading = fusionResult.state.yaw;
        }

        Nav nav{};
        nav.speed = 0_speed;
        nav.heading = targetHeading;
        if (LANDING_APPROACH_ALTITUDE + 0.1_meter < fusionResult.state.altitudeAboveGround) { // Approach
            nav.altitude = fusionResult.state.altitudeGround + LANDING_APPROACH_ALTITUDE;
            nav.stateMinor = 0;
        } else if (LANDING_SPEED < fusionResult.state.speed) { // Slow Down
            nav.altitude = fusionResult.state.altitudeAboveGround + LANDING_APPROACH_ALTITUDE;
            nav.stateMinor = 1;
        } else if (LANDING_FLARE_ALTITUDE < fusionResult.state.altitudeAboveGround) { // Landing
            nav.altitude = LANDING_FLARE_ALTITUDE - 0.1_meter;
            nav.stateMinor = 2;
        } else { // Flare
            nav.altitude = fusionResult.state.position.altitude + 100_meter;
            nav.stateMinor = 3;
        }

        nav.stateMajor = 2;
        nav.fusionResult = fusionResult;

        out.put(nav);
    }

    void Navigation::loiter(const FusionResult &fusionResult, bool reset) {
        Nav nav{};
        static auto centre = fusionResult.state.position;
        if (reset) {
            centre = fusionResult.state.position;
        }

        auto angleFromCentre = centre.angleTo(fusionResult.state.position);
        auto targetAngle = angleFromCentre + LOITER_TARGET_ANGLE;
        auto latDist = std::cos(targetAngle / 180.0F * M_PI_F) * LOITER_RADIUS;
        auto lonDist = std::sin(targetAngle / 180.0F * M_PI_F) * LOITER_RADIUS;
        Gps_t targetPosition{centre.lat + static_cast<double>(latDist / EARTH_CIRCUMFERENCE * 360),
                             centre.lon + static_cast<double>(lonDist / EARTH_CIRCUMFERENCE * 360 /
                                                              std::cos(centre.lat / 180 * M_PI_F))};

        nav.heading = fusionResult.state.position.angleTo(targetPosition);
        nav.speed = CRUISE_SPEED;
        nav.altitude = centre.altitude;

        nav.heading =

                nav.stateMajor = 3;
        nav.stateMinor = 0;
        nav.fusionResult = fusionResult;

        out.put(nav);
    }

    void Navigation::waypoint(const FusionResult &fusionResult, bool) {
        static Waypoint_t nextWaypoint(fusionResult.state.position, std::numeric_limits<si::default_type>::max(),
                                       false);
        static uint16_t waypointIndex = 0;
        Nav nav{};

        if (fusionResult.state.position.distanceTo(nextWaypoint.location) < nextWaypoint.maxDelta) {
            waypointIn.get(nextWaypoint);
            waypointIndex++;
        }

        nav.heading = fusionResult.state.position.angleTo(nextWaypoint.location);
        nav.altitude = nextWaypoint.location.altitude;

        nav.speed = CRUISE_SPEED;

        nav.stateMajor = 4;
        nav.stateMinor = waypointIndex;
        nav.fusionResult = fusionResult;

        out.put(nav);
    }
} // namespace filter
