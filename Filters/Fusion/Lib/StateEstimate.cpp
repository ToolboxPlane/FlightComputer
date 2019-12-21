/**
 * @file StateEstimate.cpp
 * @author paul
 * @date 21.12.19
 * @brief StateEstimate @TODO
 */

#include <cstdlib>
#include <ctime>
#include "StateEstimate.hpp"

StateEstimate::StateEstimate(std::size_t numberOfParticles) {
    srand(time(nullptr));
    particles.resize(numberOfParticles);
}
