//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_NAVIGATION_HPP
#define FLIGHTCOMPUTER_NAVIGATION_HPP


#include "../../../Node.hpp"
#include "../../Fusion/Node/Fusion.hpp"
#include "../Type/Nav_t.hpp"
#include "../Type/Waypoint_t.hpp"

namespace filter {
    class Navigation : public Node {
    public:
        Navigation();

        InputChannel<State_t> &getChannelStateIn();

        InputChannel<Waypoint_t> &getChannelWaypointIn();

        OutputChannel<Nav_t> &getChannelOut();

    private:
        void run() override;

        InputChannel<State_t> stateIn;
        InputChannel<Waypoint_t> waypointIn;
        OutputChannel<Nav_t> out;

        void waypoints(State_t state, bool reset = false);

        void land(State_t state, bool reset = false);

        void launch(State_t state, bool reset = false);

        void angle(State_t state, bool reset = false);

        void hold(State_t state, bool reset = false);

        static constexpr si::extended::SpeedType<> CRUISE_SPEED{20.0};
        static constexpr si::base::MeterType<> POST_LAUNCH_ALTITUDE{10.0};
        static constexpr si::extended::AccelerationType<> THROW_THRESH{2.0};
    };
}

#endif //FLIGHTCOMPUTER_NAVIGATION_HPP
