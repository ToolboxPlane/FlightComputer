//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_NAV_T_HPP
#define FLIGHTCOMPUTER_NAV_T_HPP

#include <ostream>
#include "../../Fusion/Type/State_t.hpp"

class Nav_t {
public:
    si::default_type heading;
    si::extended::Speed<> speed;
    si::base::Meter<> altitude;
    uint16_t stateMajor, stateMinor;
    State_t state;

    friend std::ostream &operator<<(std::ostream &stream, Nav_t nav) {
        stream << "Heading: " << nav.heading << "\tSpeed : " << nav.speed<< "\taltitude: " << nav.altitude;
        stream << "\tMaj: " << nav.stateMajor << "\tMin: " << nav.stateMinor;
        return stream;
    }
};


#endif //FLIGHTCOMPUTER_NAV_T_HPP
