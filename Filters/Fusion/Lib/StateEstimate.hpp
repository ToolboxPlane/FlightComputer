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
    using WeightedParticle = std::pair<system_state_t, float>;
    explicit StateEstimate(std::size_t numberOfParticles);

    auto update(const FlightControllerPackage &flightControllerPackage, const GpsMeasurement_t &gpsMeasurement)
        -> WeightedParticle;
private:
    std::vector<WeightedParticle> particles;
    si::base::Second<> lastUpdate;
};


#endif //FLIGHTCOMPUTER_STATEESTIMATE_HPP
