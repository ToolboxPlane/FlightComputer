/**
 * @file FeedbackControl.cpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#include <cmath>
#include "FeedbackControl.hpp"

namespace filter {
    using namespace si::extended;
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

    auto FeedbackControl::speedControl(State_t state, si::extended::Speed<> target) const -> double {
        if (static_cast<decltype(target)::type>(target) == 0) {
            return 0;
        }

        static si::extended::Speed<> diffSum = 0_speed;
        static auto lastDiff = target - state.speed;
        auto deltaSpeed = target - state.speed;
        diffSum += deltaSpeed; // @TODO dt

        // Change in sign, anti windup
        if (0 < static_cast<decltype(lastDiff)::type>(lastDiff * deltaSpeed)) {
            diffSum = 0_speed;
        }
        lastDiff = deltaSpeed;

        // PI-Controller (I necessary to achieve stationary accuracy)
        double speedFeedback = static_cast<double>
                (deltaSpeed * FeedbackControl::SPEED_P + diffSum * FeedbackControl::SPEED_I);

        // Feedforward Term
        double speedFeedforward = static_cast<double>(target / MAX_SPEED);

        return clamp(speedFeedback + speedFeedforward, 0.0, 1.0);
    }

    auto FeedbackControl::headingControl(State_t state, double target) const -> double {
        double headingDiff = target - state.yaw;
        headingDiff = std::fmod(headingDiff, 360);
        if (headingDiff > 180) {
            headingDiff -= 180;
        } else if (headingDiff < -180) {
            headingDiff += 360;
        }
        auto roll = headingDiff * HEADING_P;
        return clamp(roll, -MAX_ROLL, MAX_ROLL);
    }

    auto FeedbackControl::altitudeControl(State_t state, si::base::Meter<> target) const -> double {
        auto deltaHeight = target - state.altitude;
        double pitch = static_cast<decltype(deltaHeight)::type>(deltaHeight * FeedbackControl::PITCH_P);

        return clamp(pitch, -MAX_PITCH, MAX_PITCH);
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
