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
    Navigation() : ThreadModule() {}

    Channel<Nav_t> &getChannelOut();
    Channel<State_t> &getChannelStateIn();
    Channel<Waypoint_t> &getChannelWaypointIn();
private:
    void run() override;
    Channel<State_t> stateIn;
    Channel<Waypoint_t> waypointIn;
    Channel<Nav_t> out;
};


#endif //FLIGHTCOMPUTER_NAVIGATION_HPP
