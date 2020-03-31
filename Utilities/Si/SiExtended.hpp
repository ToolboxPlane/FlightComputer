/**
 * @file SiExtended.hpp
 * @author paul
 * @date 23.03.19
 * @brief SiExtended @TODO
 */

#ifndef FLIGHTCOMPUTER_SIEXTENDED_HPP
#define FLIGHTCOMPUTER_SIEXTENDED_HPP

#include <iostream>
#include "Si.hpp"
#include "SiLiterals.hpp"

namespace si {
    template <int exp>
    constexpr void printHelper(std::ostream &ostream, const char *name) {
        if constexpr (exp != 0) {
            if constexpr (exp == -1) {
                ostream << "/" << name;
            } else if constexpr (exp == 1) {
                ostream << name;
            } else {
                ostream << " " << name << "^" << exp;
            }
        }
    }

    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    std::ostream &operator<<(std::ostream& ostream, const Si<m,kg,s,A,K,MOL,CD,T> &si) {
        auto val = static_cast<T>(si);
        ostream << val;
        printHelper<m>(ostream, "m");
        printHelper<kg>(ostream, "kg");
        printHelper<s>(ostream, "s");
        printHelper<A>(ostream, "A");
        printHelper<K>(ostream, "K");
        printHelper<MOL>(ostream, "Mol");
        printHelper<CD>(ostream, "Cd");
        return ostream;
    }

    namespace extended {
        template<typename T = si::default_type>
        using Speed = Si<1, 0, -1, 0, 0, 0, 0, T>;
        constexpr Speed<> speed{1};
        CREATE_ALL_PREFIX(speed,1,0,-1,0,0,0,0)

        template<typename T = si::default_type>
        using Acceleration = Si<1, 0, -2, 0, 0, 0, 0, T>;
        constexpr Acceleration<> acceleration{1};
        CREATE_ALL_PREFIX(acceleration,1,0,-2,0,0,0,0)

        template<typename T = si::default_type>
        using Voltage = Si<2, 1, -3, -1, 0, 0, 0, T>;
        constexpr Voltage<> volt{1};
        CREATE_ALL_PREFIX(volt,2,1,-3,-1,0,0,0)

        template<typename T = si::default_type>
        using Frequency = Si<0, 0, -1, 0, 0, 0, 0, T>;
        constexpr Frequency<> hertz{1};
        CREATE_ALL_PREFIX(hertz,0,0,-1,0,0,0,0)
    }
}

#endif //FLIGHTCOMPUTER_SIEXTENDED_HPP
