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
    auto header() -> std::string;

    template<typename T>
    auto getLine(const T&) -> std::string;

    template <typename T>
    auto getItem(const std::vector<std::string>&) -> T;
}

#endif //FLIGHTCOMPUTER_SERIALIZATION_HPP
