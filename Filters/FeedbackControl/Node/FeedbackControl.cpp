/**
 * @file FeedbackControl.cpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#include "FeedbackControl.hpp"

#include <cmath>

#include "../../../Utilities/time.hpp"

namespace filter {
    // using namespace si;
    using namespace si::literals;

    FeedbackControl::FeedbackControl() {
        this->start();
    }

    void FeedbackControl::run() {
        while (!channelIn.isClosed()) {
            Nav nav{};
            channelIn.get(nav);

            Control control{};
            control.roll = headingControl(nav.fusionResult, nav.heading);
            control.power = speedControl(nav.fusionResult, nav.speed);
            control.pitch = altitudeControl(nav.fusionResult, nav.altitude);
            control.fusionResult = nav.fusionResult;

            channelOut.put(control);
        }
    }

    auto FeedbackControl::speedControl(const FusionResult &fusionResult, si::Speed<> target) const -> si::Scalar<> {
        if (target == 0.0F * si::speed || !fusionResult.loraRemote.isArmed) {
            return 0;
        }

        static si::Meter<> diffSum = 0_meter;
        static auto lastDiff = target - fusionResult.state.speed;
        static auto lastTime = util::time::get();
        auto currTime = util::time::get();
        auto dt = static_cast<si::Second<>>(lastTime - currTime);
        lastTime = currTime;

        auto deltaSpeed = target - fusionResult.state.speed;
        diffSum += deltaSpeed * dt;

        lastDiff = deltaSpeed;

        // PI-Controller ("I" necessary to achieve stationary accuracy)
        auto speedFeedback = deltaSpeed * FeedbackControl::SPEED_P + diffSum * FeedbackControl::SPEED_I;

        // Feedforward Term
        auto speedFeedforward = target / MAX_SPEED;

        return clamp<si::Scalar<>>(speedFeedback + speedFeedforward, 0.0F, 1.0F);
    }

    auto FeedbackControl::headingControl(const FusionResult &fusionResult, si::Scalar<> target) const -> si::Scalar<> {
        auto headingDiff = target - fusionResult.state.yaw;
        headingDiff = fmodf(headingDiff, 360);
        if (headingDiff > si::Scalar<>{180.0F}) {
            headingDiff -= 180.0F;
        } else if (-si::Scalar<>{180.0F} > headingDiff) {
            headingDiff += 360.0F;
        }
        auto roll = headingDiff * HEADING_P;
        return clamp(roll, -HEADING_MAX_ROLL, HEADING_MAX_ROLL);
    }

    auto FeedbackControl::altitudeControl(const FusionResult &fusionResult, si::Meter<> target) const -> si::Scalar<> {
        auto deltaHeight = target - fusionResult.state.position.altitude;
        auto pitch = deltaHeight * FeedbackControl::ALTITUDE_P;

        return clamp(pitch, -ALTITUDE_MAX_PITCH, ALTITUDE_MAX_PITCH);
    }

    InputChannel<Nav> &FeedbackControl::getChannelIn() {
        return this->channelIn;
    }

    OutputChannel<Control> &FeedbackControl::getChannelOut() {
        return this->channelOut;
    }

    template<typename T>
    auto FeedbackControl::clamp(T val, T min, T max) -> T {
        if (val < min) {
            return min;
        } else if (max < val) {
            return max;
        } else {
            return val;
        }
    }
} // namespace filter
