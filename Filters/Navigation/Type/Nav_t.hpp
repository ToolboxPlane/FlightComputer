//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_NAV_T_HPP
#define FLIGHTCOMPUTER_NAV_T_HPP

#include <ostream>

#include "../../Fusion/Type/State_t.hpp"

struct Nav {
    si::default_type heading;
    si::Speed<> speed;
    si::Meter<> altitude;
    uint16_t stateMajor, stateMinor;
    FusionResult fusionResult;
};


#endif // FLIGHTCOMPUTER_NAV_T_HPP
