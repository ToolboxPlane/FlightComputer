/**
 * @file StateEstimateParticleFilter.hpp
 * @author paul
 * @date 21.12.19
 * @brief StateEstimateParticleFilter @TODO
 */

#ifndef FLIGHTCOMPUTER_STATEESTIMATEPARTICLEFILTER_HPP
#define FLIGHTCOMPUTER_STATEESTIMATEPARTICLEFILTER_HPP

#include <vector>
#include <chrono>

extern "C" {
    #include "system.h"
}

#include "../Type/State_t.hpp"

class StateEstimateParticleFilter {
public:
    StateEstimateParticleFilter();

    auto update(const FlightControllerPackage &flightControllerPackage, const GpsMeasurement_t &gpsMeasurement)
        -> weighted_particle_t;

    static auto getCurrSeconds() -> si::base::Second<>;
private:
    std::vector<weighted_particle_t> particles;
    si::base::Second<> lastUpdate;
    std::size_t newParticleNum;
};


#endif //FLIGHTCOMPUTER_STATEESTIMATEPARTICLEFILTER_HPP
