/**
 * @file random_util.h
 * @author paul
 * @date 21.12.19
 * @brief random_util @TODO
 */

#ifndef FLIGHTCOMPUTER_RANDOM_UTIL_H
#define FLIGHTCOMPUTER_RANDOM_UTIL_H

#include "config.h"

#define FAST_RAND_MAX 15485867
#define __FAST_RAND_PERIOD 15485863

int fast_rand(void);

#define RAND_IMPL fast_rand
#define RAND_IMPL_MAX FAST_RAND_MAX

/**
 * Draw a number from a gaussian distribution
 * (stolen from:
 * https://stackoverflow.com/questions/5817490/implementing-box-mueller-random-number-generator-in-c-sharp)
 * @param mu the mean of the distribution
 * @param sigma the standard deviation of the distribution
 * @return the number
 */
real_t gaussian_box_muller(real_t mu, real_t sigma);

/**
 * Calculates the transformation matrix to transform two standard normal distributed
 * variables to a vector with the given covariance matrix.
 *
 * For this the eigenvalues (lambda_1 and lambda_2) and the eigenvectors (x_1 and x_2)
 * of the covariance matrix are calculated.
 *
 * @param sigma11 the variance of x_1
 * @param sigma12 the covariance of x_1 and x_2
 * @param sigma22 the variance of x_2
 * @param out the output matrix
 */
void get_cov_trans_mat_2d(real_t sigma11, real_t sigma12, real_t sigma22, real_t out[2][2]);

/**
 * Draw a vector from a two dimensional multivariate gaussian distribution
 * @param sigma11 the variance of x_1
 * @param sigma12 the covariance of x_1 and x_2
 * @param sigma22 the variance of x_2
 * @param x_1 the first component of the output
 * @param x_2 the second component of the output
 */
void draw_gaussian_2d(real_t sigma11, real_t sigma12, real_t sigma22, real_t *x_1, real_t *x_2);

void draw_gaussian_2d_cov(real_t trans_mat[2][2], real_t *x_1, real_t *x_2);

/**
 * Add additive white gaussian noise to the two state vectors of a constant
 * velocity model
 * @param sigma the standard deviation used for the process noise
 * @param dt the timestep
 * @param x the first state component ("position")
 * @param x_diff the second state component ("velocity")
 */
void constant_velo_awgn(real_t sigma, real_t dt, real_t *x, real_t *x_diff);

void constant_velo_awgn_cov(real_t sigma, real_t cov[2][2], real_t *x, real_t *x_diff);

/**
 * Calculates the value of the gaussian probability density function at a position
 * @param mu the mean of the gaussian distribution
 * @param sigma2 the variance (sigma^2)
 * @param x the position at which to calculate the likelihood
 * @return the value of the pdf at the given location
 */
real_t gaussian(real_t mu, real_t sigma2, real_t x);

#endif //FLIGHTCOMPUTER_RANDOM_UTIL_H
