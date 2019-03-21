/**
 * @file SiBase.hpp
 * @author paul
 * @date 21.03.19
 * @brief SiBase @TODO
 */

#ifndef FLIGHTCOMPUTER_SIBASE_HPP
#define FLIGHTCOMPUTER_SIBASE_HPP

#include "Si.hpp"

namespace si::base {
    // Base units
    const Si<1,0,0,0,0,0,0> meter{1};
    const Si<0,1,0,0,0,0,0> kilogram{1};
    const Si<0,0,1,0,0,0,0> second{1};
    const Si<0,0,0,1,0,0,0> ampere{1};
    const Si<0,0,0,0,1,0,0> kelvin{1};
    const Si<0,0,0,0,0,1,0> mole{1};
    const Si<0,0,0,0,0,0,1> candela{1};

    template <typename T = long double>
    using MeterType = Si<1,0,0,0,0,0,0,T>;
    template <typename T = long double>
    using KilogramType = Si<0,1,0,0,0,0,0,T>;
    template <typename T = long double>
    using SecondType = Si<0,0,1,0,0,0,0,T>;
    template <typename T = long double>
    using AmpereType = Si<0,0,0,1,0,0,0,T>;
    template <typename T = long double>
    using KelvinType = Si<0,0,0,0,1,0,0,T>;
    template <typename T = long double>
    using MoleType = Si<0,0,0,0,0,1,0,T>;
    template <typename T = long double>
    using CandelaType = Si<0,0,0,0,0,0,1,T>;
}

#endif //FLIGHTCOMPUTER_SIBASE_HPP
