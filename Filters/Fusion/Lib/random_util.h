/**
 * @file random_util.h
 * @author paul
 * @date 21.12.19
 * @brief random_util @TODO
 */

#ifndef FLIGHTCOMPUTER_RANDOM_UTIL_H
#define FLIGHTCOMPUTER_RANDOM_UTIL_H

/**
 * Draw a number from a gaussian distribution (stolen from wikipedia: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform)
 * @param mu the mean of the distribution
 * @param sigma the standard deviation of the distribution
 * @return the number
 */
float gaussian_box_muller(float mu, float sigma);

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
void get_cov_trans_mat_2d(float sigma11, float sigma12, float sigma22, float out[2][2]);

/**
 * Draw a vector from a two dimensional multivariate gaussian distribution
 * @param sigma11 the variance of x_1
 * @param sigma12 the covariance of x_1 and x_2
 * @param sigma22 the variance of x_2
 * @param x_1 the first component of the output
 * @param x_2 the second component of the output
 */
void draw_gaussian_2d(float sigma11, float sigma12, float sigma22, float *x_1, float *x_2);

/**
 * Add additive white gaussian noise to the two state vectors of a constant
 * velocity model
 * @param sigma the standard deviation used for the process noise
 * @param dt the timestep
 * @param x the first state component ("position")
 * @param x_diff the second state component ("velocity")
 */
void constant_velo_awgn(float sigma, float dt, float *x, float *x_diff);

float gaussian(float mu, float sigma2, float x);

#endif //FLIGHTCOMPUTER_RANDOM_UTIL_H
