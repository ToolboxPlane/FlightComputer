/**
 * @file FeedbackControl.hpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#ifndef FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP
#define FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP

#include "../../Node.hpp"
#include "../../InputChannel.hpp"
#include "../Navigation/Nav_t.hpp"
#include "../../OutputChannel.hpp"
#include "Control_t.hpp"

namespace filter {
    class FeedbackControl : public Node {
    public:
        InputChannel<Nav_t> &getChannelIn();
        OutputChannel<Control_t> &getChannelOut();
    private:
        void run() override;

        auto speedControl(State_t state, double target) -> double;
        auto headingControl(State_t state, double target) -> double;
        auto altitudeControl(State_t state, double target) -> double;

        InputChannel<Nav_t> channelIn;
        OutputChannel<Control_t> channelOut;

        static constexpr auto SPEED_P = 1.0;
        static constexpr auto PITCH_P = 1.0;
        static constexpr auto MAX_ROLL = 80.0;
        static constexpr auto MAX_PITCH = 60.0;
        static constexpr auto HEADING_P = 1.0;
        static constexpr auto SPEED_I = 0.5;
    };
}

#endif //FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP
