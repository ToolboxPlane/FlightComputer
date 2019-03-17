/**
 * @file FeedbackControl.cpp
 * @author paul
 * @date 17.03.19
 * @brief FeedbackControl @TODO
 */

#include <cmath>
#include "FeedbackControl.hpp"

namespace filter {
    void FeedbackControl::run() {
        while (!channelIn.isClosed()) {
            Nav_t nav{};
            channelIn.get(nav);

            Control_t control{};
            control.pitch = headingControl(nav.state, nav.heading);
            control.power = speedControl(nav.state, nav.speed);
            control.roll = altitudeControl(nav.state, nav.altitude);

            channelOut.put(control);
        }
    }

    auto FeedbackControl::speedControl(State_t state, double target) -> double {
        if (target == 0) {
            return 0;
        }

        static double diffSum = 0;
        static double lastDiff = target - state.groundSpeed;
        double deltaSpeed = target - state.groundSpeed;
        diffSum += deltaSpeed;

        // Change in sign, anti windup
        if (lastDiff * deltaSpeed < 0) {
            diffSum = 0;
        }
        lastDiff = deltaSpeed;

        // PI-Controller (I necessary to achieve stationary accuracy)
        double speed = deltaSpeed * FeedbackControl::SPEED_P + diffSum * FeedbackControl::SPEED_I;

        if (speed < 0.0) {
            speed = 0;
        } else if (speed > 1.0) {
            speed = 1.0;
        }
        return speed;
    }

    auto FeedbackControl::headingControl(State_t state, double target) -> double {
        double headingDiff = target - state.heading;
        headingDiff = std::fmod(headingDiff, 360);
        if (headingDiff > 180) {
            headingDiff -= 180;
        } else if (headingDiff < -180) {
            headingDiff += 360;
        }
        double roll = -headingDiff * HEADING_P; // sign is a result of compass angles not being mathematically positive
        if (roll < -MAX_ROLL) {
            roll = -MAX_ROLL;
        } else if (roll > MAX_ROLL) {
            roll = MAX_ROLL;
        }
        return roll;
    }

    auto FeedbackControl::altitudeControl(State_t state, double target) -> double {
        double deltaHeight = state.heightAboveSeaLevel - target;
        double pitch = deltaHeight * FeedbackControl::PITCH_P;

        if (pitch < -MAX_PITCH) {
            pitch = -MAX_PITCH;
        } else if (pitch > MAX_PITCH) {
            pitch = MAX_PITCH;
        }

        return pitch;
    }

    InputChannel<Nav_t> &FeedbackControl::getChannelIn() {
        return this->channelIn;
    }

    OutputChannel<Control_t> &FeedbackControl::getChannelOut() {
        return this->channelOut;
    }

}