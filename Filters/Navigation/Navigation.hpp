//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_NAVIGATION_HPP
#define FLIGHTCOMPUTER_NAVIGATION_HPP


#include "../../ThreadModule.hpp"
#include "../Fusion/Fusion.hpp"
#include "Nav_t.hpp"
#include "Waypoint_t.hpp"

class Navigation : public ThreadModule{
public:
    Navigation();

    Channel<State_t> &getChannelStateIn();
    Channel<Waypoint_t> &getChannelWaypointIn();
    MultipleOutputChannel<Nav_t> &getChannelOut();
private:
    void run() override;
    Channel<State_t> stateIn;
    Channel<Waypoint_t> waypointIn;
    MultipleOutputChannel<Nav_t> out;

    void waypoints(State_t state, bool reset = false);
    void land(State_t state, bool reset = false);
    void launch(State_t state, bool reset = false);
    void angle(State_t state, bool reset = false);
    void hold(State_t state, bool reset = false);

    auto speedControl(double airspeed, double target = CRUISE_SPEED) -> double;
    auto headingControl(double currHeading, double target) -> double;

    static constexpr auto CRUISE_SPEED = 40.0;
    static constexpr auto SPEED_P = 1.0;
    static constexpr auto PITCH_P = 1.0;
    static constexpr auto MAX_ROLL = 90.0;
    static constexpr auto MAX_PITCH = 60.0;
    static constexpr auto HEADING_P = 0.5;
    static constexpr auto POST_LAUNCH_CLIMB = 20;
    static constexpr auto THROW_THRESH = 2.0;
    static constexpr auto SPEED_I = 0.5;
};


#endif //FLIGHTCOMPUTER_NAVIGATION_HPP
