/**
 * @file time.cpp
 * @author paul
 * @date 06.04.20
 * Description here TODO
 */
#include "time.hpp"

#include "chrono"

auto util::time::get() -> si::Second<long double> {
    auto tp = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto microseconds = static_cast<long double>(
            std::chrono::duration_cast<std::chrono::microseconds>(tp).count());
    return si::Second<long double>(microseconds / 10e6);
}
