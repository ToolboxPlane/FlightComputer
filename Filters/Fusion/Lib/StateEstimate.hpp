/**
 * @file StateEstimate.hpp
 * @author paul
 * @date 21.12.19
 * @brief StateEstimate @TODO
 */

#ifndef FLIGHTCOMPUTER_STATEESTIMATE_HPP
#define FLIGHTCOMPUTER_STATEESTIMATE_HPP

#include <vector>
extern "C" {
    #include "system.h"
}

class StateEstimate {
public:
    explicit StateEstimate(std::size_t numberOfParticles);

private:
    std::vector<system_state_t> particles;
};


#endif //FLIGHTCOMPUTER_STATEESTIMATE_HPP
