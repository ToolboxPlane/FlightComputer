/**
 * @file SiBase.hpp
 * @author paul
 * @date 21.03.19
 * @brief SiBase @TODO
 */

#ifndef FLIGHTCOMPUTER_SIBASE_HPP
#define FLIGHTCOMPUTER_SIBASE_HPP

#include "Si.hpp"

// ...
#define CREATE_ALL_PREFIX(BASE_NAME, a, b, c, d, e, f, g) \
constexpr Si<a,b,c,d,e,f,g> operator "" _Y_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E24}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _Z_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E21}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _E_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E18}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _P_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E15}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _T_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E12}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _G_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E9}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _M_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E6}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _k_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E3}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _h_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E2}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _da_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E1}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _d_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-1}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _c_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-2}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _m_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-3}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _mu_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-6}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _n_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-9}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _p_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-12}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _f_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-15}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _a_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-18}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _z_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-21}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _y_##BASE_NAME(long double t) { return Si<a,b,c,d,e,f,g>{t * 1E-24}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _Y_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E24}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _Z_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E21}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _E_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E18}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _P_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E15}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _T_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E12}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _G_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E9}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _M_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E6}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _k_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E3}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _h_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E2}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _da_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E1}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t)}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _d_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-1}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _c_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-2}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _m_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-3}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _mu_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-6}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _n_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-9}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _p_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-12}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _f_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-15}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _a_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-18}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _z_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-21}; } \
constexpr Si<a,b,c,d,e,f,g> operator "" _y_##BASE_NAME(unsigned long long t) { return Si<a,b,c,d,e,f,g>{static_cast<long double>(t) * 1E-24}; }

namespace si::base {
    // Base types
    template<typename T = long double>
    using MeterType = Si<1, 0, 0, 0, 0, 0, 0, T>;
    template<typename T = long double>
    using KilogramType = Si<0, 1, 0, 0, 0, 0, 0, T>;
    template<typename T = long double>
    using SecondType = Si<0, 0, 1, 0, 0, 0, 0, T>;
    template<typename T = long double>
    using AmpereType = Si<0, 0, 0, 1, 0, 0, 0, T>;
    template<typename T = long double>
    using KelvinType = Si<0, 0, 0, 0, 1, 0, 0, T>;
    template<typename T = long double>
    using MoleType = Si<0, 0, 0, 0, 0, 1, 0, T>;
    template<typename T = long double>
    using CandelaType = Si<0, 0, 0, 0, 0, 0, 1, T>;

    // Base units
    constexpr MeterType<> meter{1};
    constexpr KilogramType<> kilogram{1};
    constexpr SecondType<> second{1};
    constexpr AmpereType<> ampere{1};
    constexpr KelvinType<> kelvin{1};
    constexpr MoleType<> mole{1};
    constexpr CandelaType<> candela{1};
}

namespace si::literals {
    CREATE_ALL_PREFIX(meter,1,0,0,0,0,0,0)
    CREATE_ALL_PREFIX(kilogram, 0,1,0,0,0,0,0)
    CREATE_ALL_PREFIX(second,0,0,1,0,0,0,0)
    CREATE_ALL_PREFIX(ampere,0,0,0,1,0,0,0)
    CREATE_ALL_PREFIX(kelvin,0,0,0,0,1,0,0)
    CREATE_ALL_PREFIX(mole,0,0,0,0,0,1,0)
    CREATE_ALL_PREFIX(candela,0,0,0,0,0,0,1)

    // Prefix for double
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
    // Prefix for int
    constexpr long double operator "" _Y(unsigned long long t) { return t * 1E24; }
    constexpr long double operator "" _Z(unsigned long long t) { return t * 1E21; }
    constexpr long double operator "" _E(unsigned long long t) { return t * 1E18; }
    constexpr long double operator "" _P(unsigned long long t) { return t * 1E15; }
    constexpr long double operator "" _T(unsigned long long t) { return t * 1E12; }
    constexpr long double operator "" _G(unsigned long long t) { return t * 1E9; }
    constexpr long double operator "" _M(unsigned long long t) { return t * 1E6; }
    constexpr long double operator "" _k(unsigned long long t) { return t * 1E3; }
    constexpr long double operator "" _h(unsigned long long t) { return t * 1E2; }
    constexpr long double operator "" _da(unsigned long long t) { return t * 1E1; }
    constexpr long double operator "" _d(unsigned long long t) { return t * 1E-1; }
    constexpr long double operator "" _c(unsigned long long t) { return t * 1E-2; }
    constexpr long double operator "" _m(unsigned long long t) { return t * 1E-3; }
    constexpr long double operator "" _mu(unsigned long long t) { return t * 1E-6; }
    constexpr long double operator "" _n(unsigned long long t) { return t * 1E-9; }
    constexpr long double operator "" _p(unsigned long long t) { return t * 1E-12; }
    constexpr long double operator "" _f(unsigned long long t) { return t * 1E-15; }
    constexpr long double operator "" _a(unsigned long long t) { return t * 1E-18; }
    constexpr long double operator "" _z(unsigned long long t) { return t * 1E-21; }
    constexpr long double operator "" _y(unsigned long long t) { return t * 1E-24; }
}

#undef CREATE_ALL_PREFIX
#endif //FLIGHTCOMPUTER_SIBASE_HPP
