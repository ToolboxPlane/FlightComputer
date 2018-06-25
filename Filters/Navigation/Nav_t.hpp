//
// Created by paul on 17.03.18.
//

#ifndef FLIGHTCOMPUTER_NAV_T_HPP
#define FLIGHTCOMPUTER_NAV_T_HPP

#include <ostream>

class Nav_t {
public:
    double pitch, roll, power;
    friend std::ostream &operator<<(std::ostream &stream, Nav_t nav) {
        stream << "Pitch: " << nav.pitch << "\tRoll: " << nav.roll << "\tPower: " << nav.power;
        return stream;
    }
};


#endif //FLIGHTCOMPUTER_NAV_T_HPP
