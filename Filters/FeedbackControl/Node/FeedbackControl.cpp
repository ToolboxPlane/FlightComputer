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
            Nav_t nav{};
            channelIn.get(nav);

            Control_t control{};
            control.roll = headingControl(nav.state, nav.heading);
            control.power = speedControl(nav.state, nav.speed);
            control.pitch = altitudeControl(nav.state, nav.altitude);
            control.state = nav.state;

            channelOut.put(control);
        }
    }

    auto FeedbackControl::speedControl(const State_t &state, si::Speed<> target) const -> si::Scalar<> {
        if (target == 0.0F * si::speed || !state.loraRemote.isArmed) {
            return 0;
        }

        static si::Meter<> diffSum = 0_meter;
        static auto lastDiff = target - state.speed;
        static auto lastTime = util::time::get();
        auto currTime = util::time::get();
        auto dt = static_cast<si::Second<>>(lastTime - currTime);
        lastTime = currTime;

        auto deltaSpeed = target - state.speed;
        diffSum += deltaSpeed * dt;

        lastDiff = deltaSpeed;

        // PI-Controller (I necessary to achieve stationary accuracy)
        auto speedFeedback = deltaSpeed * FeedbackControl::SPEED_P + diffSum * FeedbackControl::SPEED_I;

        // Feedforward Term
        auto speedFeedforward = target / MAX_SPEED;

        return clamp<si::Scalar<>>(speedFeedback + speedFeedforward, 0.0F, 1.0F);
    }

    auto FeedbackControl::headingControl(const State_t &state, si::Scalar<> target) const -> si::Scalar<> {
        auto headingDiff = target - state.yaw;
        headingDiff = fmodf(headingDiff, 360);
        if (headingDiff > si::Scalar<>{180.0F}) {
            headingDiff -= 180.0F;
        } else if (-si::Scalar<>{180.0F} > headingDiff) {
            headingDiff += 360.0F;
        }
        auto roll = headingDiff * HEADING_P;
        return clamp(roll, -HEADING_MAX_ROLL, HEADING_MAX_ROLL);
    }

    auto FeedbackControl::altitudeControl(const State_t &state, si::Meter<> target) const -> si::Scalar<> {
        auto deltaHeight = target - state.position.altitude;
        auto pitch = deltaHeight * FeedbackControl::ALTITUDE_P;

        return clamp(pitch, -ALTITUDE_MAX_PITCH, ALTITUDE_MAX_PITCH);
    }

    InputChannel<Nav_t> &FeedbackControl::getChannelIn() {
        return this->channelIn;
    }

    OutputChannel<Control_t> &FeedbackControl::getChannelOut() {
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
