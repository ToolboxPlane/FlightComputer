/**
 * @file StateEstimate.hpp
 * @author paul
 * @date 21.12.19
 * @brief StateEstimate @TODO
 */

#ifndef FLIGHTCOMPUTER_STATEESTIMATE_HPP
#define FLIGHTCOMPUTER_STATEESTIMATE_HPP

#include <vector>
#include <chrono>

extern "C" {
    #include "system.h"
}

#include "../Type/State_t.hpp"

class StateEstimate {
public:
    explicit StateEstimate(std::size_t numberOfParticles, std::size_t resamplePeriod=1);

    auto update(const FlightControllerPackage &flightControllerPackage, const GpsMeasurement_t &gpsMeasurement)
        -> weighted_particle_t;

    static auto getCurrSeconds() -> si::base::Second<>;
private:
    std::vector<weighted_particle_t> particles;
    si::base::Second<> lastUpdate;
    std::size_t resamplePeriod;
    std::size_t resampleStep;
};


#endif //FLIGHTCOMPUTER_STATEESTIMATE_HPP
