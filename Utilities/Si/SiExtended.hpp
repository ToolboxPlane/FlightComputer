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

namespace si {
    template <int exp>
    constexpr void printHelper(std::ostream &ostream, const char *name) {
        if constexpr (exp != 0) {
            if constexpr (exp == -1) {
                ostream << " /" << name;
            } else if constexpr (exp == 1) {
                ostream << " " << name;
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

}

#endif //FLIGHTCOMPUTER_SIEXTENDED_HPP
