/**
 * @file random_util.h
 * @author paul
 * @date 21.12.19
 * @brief random_util @TODO
 */

#ifndef FLIGHTCOMPUTER_RANDOM_UTIL_H
#define FLIGHTCOMPUTER_RANDOM_UTIL_H

#include "config.h"

/**
 * Draw a number from a gaussian distribution
 * (stolen from:
 * https://stackoverflow.com/questions/5817490/implementing-box-mueller-random-number-generator-in-c-sharp)
 * @param mu the mean of the distribution
 * @param sigma the standard deviation of the distribution
 * @return the number
 */
float gaussian_box_muller(float mu, float sigma);

/**
 * Calculates the value of the gaussian probability density function at a position
 * @param mu the mean of the gaussian distribution
 * @param sigma the standard deviation
 * @param x the position at which to calculate the likelihood
 * @return the value of the pdf at the given location
 */
float gaussian(float mu, float sigma, float x);

#endif //FLIGHTCOMPUTER_RANDOM_UTIL_H
