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

            void waypoint(const State_t &currentState, bool = false);

            void land(const State_t &state, bool reset = false);

            void launch(const State_t &state, bool reset = false);

            void rth(const State_t &currentState, bool = false);

            void loiter(const State_t &state, bool reset = false);

            static constexpr si::extended::Speed<> CRUISE_SPEED{20.0};
            static constexpr si::base::Meter<> POST_LAUNCH_ALTITUDE{10.0};
            static constexpr si::extended::Acceleration<> THROW_THRESH{2.0};
            static constexpr si::extended::Speed<> LANDING_SPEED{5.0};
            static constexpr si::base::Meter<> LANDING_APPROACH_ALT{2.5};
            static constexpr si::base::Meter<> LANDING_FLARE_ALT{0.5};
            static constexpr si::default_type LAUNCH_MAX_ROLL{30};
            static constexpr si::default_type LAUNCH_MIN_PITCH{0};
            static constexpr si::default_type LAUNCH_MAX_PITCH{90};
            static constexpr si::base::Meter<> RTH_ALT{10};
            static constexpr si::base::Meter<> LOITER_RADIUS{20};
            static constexpr si::default_type LOITER_TARGET_ANGLE{72};
    };
}

#endif //FLIGHTCOMPUTER_NAVIGATION_HPP
