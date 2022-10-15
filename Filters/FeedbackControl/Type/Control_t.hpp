/**
 * @file Control_t.hpp
 * @author paul
 * @date 17.03.19
 * @brief Control_t @TODO
 */

#ifndef FLIGHTCOMPUTER_CONTROL_T_HPP
#define FLIGHTCOMPUTER_CONTROL_T_HPP

#include <ostream>

#include "../../Fusion/Type/State_t.hpp"

class Control_t {
  public:
    float pitch, roll, power;
    State_t state;

    friend std::ostream &operator<<(std::ostream &stream, Control_t control) {
        stream << "Pitch: " << control.pitch << "\tRoll: " << control.roll << "\tPower: " << control.power;
        return stream;
    }
};

#endif // FLIGHTCOMPUTER_CONTROL_T_HPP
