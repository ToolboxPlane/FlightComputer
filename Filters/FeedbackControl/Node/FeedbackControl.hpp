/**
 * @file FeedbackControl.hpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#ifndef FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP
#define FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP

#include "../../../Node.hpp"
#include "../../../InputChannel.hpp"
#include "../../Navigation/Type/Nav_t.hpp"
#include "../../../OutputChannel.hpp"
#include "../Type/Control_t.hpp"

namespace filter {
    class FeedbackControl : public Node {
        public:
            FeedbackControl();

            auto getChannelIn() -> InputChannel<Nav_t> &;

            auto getChannelOut() -> OutputChannel<Control_t> &;

        private:
            void run() override;

            [[nodiscard]] auto
            speedControl(const State_t &state, si::extended::Speed<> target) const -> si::default_type;

            [[nodiscard]] auto headingControl(const State_t &state, si::default_type target) const -> si::default_type;

            [[nodiscard]] auto
            altitudeControl(const State_t &state, si::base::Meter<> target) const -> si::default_type;

            template<typename T>
            static auto clamp(T val, T min, T max) -> T;

            InputChannel<Nav_t> channelIn;
            OutputChannel<Control_t> channelOut;

            static constexpr auto SPEED_P = 1.0F / si::extended::speed;
            static constexpr auto PITCH_P = 1.0F / si::base::meter;
            static constexpr auto MAX_ROLL = 80.0F;
            static constexpr auto MAX_PITCH = 30.0F;
            static constexpr auto HEADING_P = 1.0F;
            static constexpr auto SPEED_I = 0.5F / si::extended::speed;
            static constexpr auto MAX_SPEED = 100 / 3.6F * si::extended::speed;
    };
}

#endif //FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP
