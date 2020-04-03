/**
 * @file Serialization.hpp
 * @author paul
 * @date 03.04.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_SERIALIZATION_HPP
#define FLIGHTCOMPUTER_SERIALIZATION_HPP

#include <string>
#include <vector>

namespace recording {
    template<typename T>
    auto header() -> std::string {
        static_assert("header() not defined for class");
        return "";
    }

    template<typename T>
    auto getLine(const T&) -> std::string {
        static_assert("getLine() not defined for class");
        return "";
    }

    template <typename T>
    auto getItem(const std::vector<std::string>&) -> T {
        static_assert("getItem() not defined for class");
        return T{};
    }

}

#endif //FLIGHTCOMPUTER_SERIALIZATION_HPP
