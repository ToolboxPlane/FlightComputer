/**
 * @file FeedbackControl.hpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#ifndef FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP
#define FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP

#include "../../../Framework/InputChannel.hpp"
#include "../../../Framework/Node.hpp"
#include "../../../Framework/OutputChannel.hpp"
#include "../../Navigation/Type/Nav_t.hpp"
#include "../Type/Control_t.hpp"

namespace filter {
    class FeedbackControl : public Node {
      public:
        FeedbackControl();

        auto getChannelIn() -> InputChannel<Nav> &;

        auto getChannelOut() -> OutputChannel<Control> &;

      private:
        void run() override;

        [[nodiscard]] auto speedControl(const FusionResult &fusionResult, si::Speed<> target) const -> si::Scalar<>;

        [[nodiscard]] auto headingControl(const FusionResult &fusionResult, si::Scalar<> target) const -> si::Scalar<>;

        [[nodiscard]] auto altitudeControl(const FusionResult &fusionResult, si::Meter<> target) const -> si::Scalar<>;

        template<typename T>
        static auto clamp(T val, T min, T max) -> T;

        InputChannel<Nav> channelIn;
        OutputChannel<Control> channelOut;

        static constexpr auto SPEED_P = 1.0F / si::speed;
        static constexpr auto SPEED_I = 0.5F / si::meter;
        static constexpr auto ALTITUDE_P = 1.0F / si::meter;
        static constexpr si::Scalar<> ALTITUDE_MAX_PITCH = 30.0F;
        static constexpr si::Scalar<> HEADING_MAX_ROLL = 60.0F;
        static constexpr si::Scalar<> HEADING_P = 2.0F;
        static constexpr auto MAX_SPEED = 100 / 3.6F * si::speed;
    };
} // namespace filter

#endif // FLIGHTCOMPUTER_FEEDBACKCONTROL_HPP
