//
// Created by paul on 14.03.18.
//

#ifndef FLIGHTCOMPUTER_NAVIGATION_HPP
#define FLIGHTCOMPUTER_NAVIGATION_HPP


#include "../../../Framework/Node.hpp"
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

            static constexpr si::Speed<> CRUISE_SPEED{20.0};

            static constexpr si::Meter<> RTH_ALTITUDE{10};

            static constexpr si::Acceleration<> LAUNCH_THROW_THRESH{6.0};
            static constexpr si::Scalar<> LAUNCH_MAX_ROLL{30};
            static constexpr si::Scalar<> LAUNCH_MIN_PITCH{0};
            static constexpr si::Scalar<> LAUNCH_MAX_PITCH{70};
            static constexpr si::Meter<> LAUNCH_TARGET_ALTITUDE{10.0};

            static constexpr si::Speed<> LANDING_SPEED{5.0};
            static constexpr si::Meter<> LANDING_APPROACH_ALTITUDE{2.5};
            static constexpr si::Meter<> LANDING_FLARE_ALTITUDE{0.5};

            static constexpr si::Meter<> LOITER_RADIUS{20};
            static constexpr si::Scalar<> LOITER_TARGET_ANGLE{72};
    };
}

#endif //FLIGHTCOMPUTER_NAVIGATION_HPP
