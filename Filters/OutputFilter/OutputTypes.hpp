/**
 * @file OutputTypes.hpp
 * @author paul
 * @date 22.10.22
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_OUTPUTTYPES_HPP
#define FLIGHTCOMPUTER_OUTPUTTYPES_HPP

#include <Si.hpp>

struct FlightControllerSetpoint {
    si::Scalar<> power, pitch, roll;
};

#endif // FLIGHTCOMPUTER_OUTPUTTYPES_HPP
