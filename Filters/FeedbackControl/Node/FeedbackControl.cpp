/**
 * @file FeedbackControl.cpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#include <cmath>
#include "FeedbackControl.hpp"
#include "../../../Utilities/time.hpp"

namespace filter {
    using namespace si;
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

    auto FeedbackControl::speedControl(const State_t &state, si::Speed<> target) const -> si::default_type {
        if (target == 0 * si::speed || !state.loraRemote.isArmed ) {
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
        si::default_type speedFeedback = static_cast<si::default_type>
                (deltaSpeed * FeedbackControl::SPEED_P + diffSum * FeedbackControl::SPEED_I);

        // Feedforward Term
        auto speedFeedforward = static_cast<si::default_type>(target / MAX_SPEED);

        return clamp(speedFeedback + speedFeedforward, 0.0F, 1.0F);
    }

    auto FeedbackControl::headingControl(const State_t &state, si::default_type target) const -> si::default_type {
        auto headingDiff = target - state.yaw;
        headingDiff = fmodf(headingDiff, 360);
        if (headingDiff > 180) {
            headingDiff -= 180;
        } else if (headingDiff < -180) {
            headingDiff += 360;
        }
        auto roll = headingDiff * HEADING_P;
        return clamp(roll, -HEADING_MAX_ROLL, HEADING_MAX_ROLL);
    }

    auto FeedbackControl::altitudeControl(const State_t &state, si::Meter<> target) const -> si::default_type {
        auto deltaHeight = target - state.position.altitude;
        auto pitch = static_cast<decltype(deltaHeight)::type>(deltaHeight * FeedbackControl::ALTITUDE_P);

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
}
