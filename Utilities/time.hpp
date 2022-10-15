/**
 * @file time.hpp
 * @author paul
 * @date 06.04.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_TIME_HPP
#define FLIGHTCOMPUTER_TIME_HPP

#include <SiBase.hpp>

namespace util::time {
    auto get() -> si::Second<long double>;
}

#endif // FLIGHTCOMPUTER_TIME_HPP
