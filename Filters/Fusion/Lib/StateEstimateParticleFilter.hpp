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

        auto update(si::Second<> dt, const FlightControllerPackage &flightControllerPackage,
                    const GpsMeasurement_t &gpsMeasurement, const NavPackage &navPackage,
                    si::Meter<> additionalBaroUncertainty)
        -> system_state_t;

    private:
        void init(std::size_t numberOfParticles, const GpsMeasurement_t &gpsMeasurement,
                  si::Meter<> distanceGround);

        std::vector<weighted_particle_t> particles;
};


#endif //FLIGHTCOMPUTER_STATEESTIMATEPARTICLEFILTER_HPP
