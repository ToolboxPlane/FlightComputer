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

class Control {
  public:
    float pitch, roll, power;
    FusionResult fusionResult;
};

#endif // FLIGHTCOMPUTER_CONTROL_T_HPP
