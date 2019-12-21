#include "random_util.h"

#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

float gaussian_box_muller(float mu, float sigma) {
    static const float epsilon = FLT_MAX;
    static const float two_pi = 2.0f*3.14159265358979323846f;

    static float z1;
    static bool generate;
    generate = !generate;

    if (!generate) {
        return z1 * sigma + mu;
    }

    float u1, u2;
    do
    {
        u1 = (float)rand() * (1.0f / RAND_MAX);
        u2 = (float)rand() * (1.0f / RAND_MAX);
    }
    while ( u1 <= epsilon );

    float z0;
    z0 = sqrtf(-2.0f * logf(u1)) * cosf(two_pi * u2);
    z1 = sqrtf(-2.0f * logf(u1)) * sinf(two_pi * u2);
    return z0 * sigma + mu;
}

void get_cov_trans_mat_2d(float sigma11, float sigma12, float sigma22, float out[2][2]) {
    /*
     * \Sigma = [sigma11, sigma12; sigma12; sigma22]
     * -> Eigenvalues \lambda_1, \lambda_2
     */
    float tmp = sqrtf(sigma11 * sigma11 + sigma22 * sigma22 - 2 * sigma11 * sigma22 + 4 * sigma12);
    float lambda_1 = (sigma11 + sigma22 + tmp) / 2.0f; // First eigenvalue
    float lambda_2 = (sigma11 + sigma22 - tmp) / 2.0f; // Second eigenvalue

    // First eigenvector
    float v11 = 1;
    float v21 = v11 * (lambda_1 - sigma11) / sigma12;

    // Second eigenvector
    float v12 = 1;
    float v22 = v12 * (lambda_2 - sigma11) / sigma12;

    // Calculate sqrt(D) (D is diagonal)
    float sigma_1 = sqrtf(lambda_1);
    float sigma_2 = sqrtf(lambda_2);

    // Calculate V * sqrt(D)
    out[0][0] = v11 * sigma_1;
    out[1][0] = v21 * sigma_1;
    out[0][1] = v12 * sigma_2;
    out[1][1] = v22 * sigma_2;
}

void draw_gaussian_2d(float sigma11, float sigma12, float sigma22, float *x_1, float *x_2) {
    float trans_mat[2][2];
    get_cov_trans_mat_2d(sigma11, sigma12, sigma22, trans_mat);
    float x_normal_1 = gaussian_box_muller(0, 1);
    float x_normal_2 = gaussian_box_muller(0, 1);
    *x_1 = trans_mat[0][0] * x_normal_1 + trans_mat[0][1] * x_normal_2;
    *x_2 = trans_mat[1][0] * x_normal_1 + trans_mat[1][1] * x_normal_2;
}

void constant_velo_awgn(float sigma, float dt, float *x, float *x_diff) {
    float dt2 = dt*dt;
    float dt3 = dt2 * dt;
    float dt4 = dt2 * dt2;
    float x_noise, x_diff_noise;
    float sigma2 = sigma * sigma;
    draw_gaussian_2d(dt4 * sigma2 / 4, dt3 * sigma2 / 2, dt2, &x_noise, &x_diff_noise);
    *x += x_noise;
    *x_diff += x_diff_noise;
}

