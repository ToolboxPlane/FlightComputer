/**
 * @file SiLiterals.hpp
 * @author paul
 * @date 21.03.19
 * @brief SiLiterals @TODO
 */

#ifndef FLIGHTCOMPUTER_SILITERALS_HPP
#define FLIGHTCOMPUTER_SILITERALS_HPP

#include "Si.hpp"

namespace si::literals {
    // Prefix
    constexpr long double operator "" _Y(long double t) { return t * 1E24; }
    constexpr long double operator "" _Z(long double t) { return t * 1E21; }
    constexpr long double operator "" _E(long double t) { return t * 1E18; }
    constexpr long double operator "" _P(long double t) { return t * 1E15; }
    constexpr long double operator "" _T(long double t) { return t * 1E12; }
    constexpr long double operator "" _G(long double t) { return t * 1E9; }
    constexpr long double operator "" _M(long double t) { return t * 1E6; }
    constexpr long double operator "" _k(long double t) { return t * 1E3; }
    constexpr long double operator "" _h(long double t) { return t * 1E2; }
    constexpr long double operator "" _da(long double t) { return t * 1E1; }
    constexpr long double operator "" _d(long double t) { return t * 1E-1; }
    constexpr long double operator "" _c(long double t) { return t * 1E-2; }
    constexpr long double operator "" _m(long double t) { return t * 1E-3; }
    constexpr long double operator "" _mu(long double t) { return t * 1E-6; }
    constexpr long double operator "" _n(long double t) { return t * 1E-9; }
    constexpr long double operator "" _p(long double t) { return t * 1E-12; }
    constexpr long double operator "" _f(long double t) { return t * 1E-15; }
    constexpr long double operator "" _a(long double t) { return t * 1E-18; }
    constexpr long double operator "" _z(long double t) { return t * 1E-21; }
    constexpr long double operator "" _y(long double t) { return t * 1E-24; }

    constexpr Si<1,0,0,0,0,0,0> operator "" _meter(long double t) { return Si<1,0,0,0,0,0,0>{t};}
}

#endif //FLIGHTCOMPUTER_SILITERALS_HPP
