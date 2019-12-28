#include "random_util.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <values.h>

int fast_rand(void) {
    static int c = 1;
    c = (c * __FAST_RAND_PERIOD) % FAST_RAND_MAX;
    return c;
}

real_t gaussian_box_muller(real_t mu, real_t sigma) {
    static real_t v, fac;
    static int phase = 0;
    real_t S, Z, U1, U2, u;

    if (phase) {
        Z = v * fac;
    } else {
        do {
            U1 = (real_t)RAND_IMPL() / RAND_IMPL_MAX;
            U2 = (real_t)RAND_IMPL() / RAND_IMPL_MAX;

            u = 2.f * U1 - 1.f;
            v = 2.f * U2 - 1.f;
            S = u * u + v * v;
        } while (S >= 1);

        fac = sqrtf (-2.f * logf(S) / S);
        Z = u * fac;
    }

    phase = 1 - phase;

    return Z * sigma + mu;
}

void get_cov_trans_mat_2d(real_t sigma11, real_t sigma12, real_t sigma22, real_t out[2][2]) {
    assert(fabs(sigma12) <= sqrtf(sigma11 * sigma22) + FLT_EPSILON);
    /*
     * \Sigma = [sigma11, sigma12; sigma12; sigma22]
     * -> Eigenvalues \lambda_1, \lambda_2
     */
    real_t sigma_diff = sigma11 - sigma22;
    real_t tmp = sqrtf(sigma_diff * sigma_diff + 4 * sigma12 * sigma12);
    real_t lambda_1 = (sigma11 + sigma22 + tmp) / 2.0f; // First eigenvalue
    real_t lambda_2 = (sigma11 + sigma22 - tmp) / 2.0f; // Second eigenvalue

    // First eigenvector
    real_t v11 = 1;
    real_t v21 = v11 * (lambda_1 - sigma11) / sigma12;

    // Second eigenvector
    real_t v12 = 1;
    real_t v22 = v12 * (lambda_2 - sigma11) / sigma12;

    // Calculate sqrt(D) (D is diagonal)
    real_t sigma_1 = sqrtf(lambda_1);
    real_t sigma_2 = sqrtf(lambda_2);

    // Calculate V * sqrt(D)
    out[0][0] = v11 * sigma_1;
    out[1][0] = v21 * sigma_1;
    out[0][1] = v12 * sigma_2;
    out[1][1] = v22 * sigma_2;
}

void draw_gaussian_2d(real_t sigma11, real_t sigma12, real_t sigma22, real_t *x_1, real_t *x_2) {
    real_t trans_mat[2][2];
    get_cov_trans_mat_2d(sigma11, sigma12, sigma22, trans_mat);
    draw_gaussian_2d_cov(trans_mat, x_1, x_2);
}

void draw_gaussian_2d_cov(real_t trans_mat[2][2], real_t *x_1, real_t *x_2) {
    real_t x_normal_1 = gaussian_box_muller(0, 1);
    real_t x_normal_2 = gaussian_box_muller(0, 1);
    *x_1 = trans_mat[0][0] * x_normal_1 + trans_mat[0][1] * x_normal_2;
    *x_2 = trans_mat[1][0] * x_normal_1 + trans_mat[1][1] * x_normal_2;
}

void constant_velo_awgn(real_t sigma, real_t dt, real_t *x, real_t *x_diff) {
    if (dt > 0) {
        real_t dt2 = dt * dt;
        real_t dt3 = dt2 * dt;
        real_t dt4 = dt2 * dt2;
        real_t x_noise, x_diff_noise;
        real_t sigma2 = sigma * sigma;
        // Lets just assume 0.99 = 1, else our eigenvalues are negative (because of ieee754)
        draw_gaussian_2d(dt4 / 4.f * sigma2, dt3 / 2.f * sigma2 * 0.99f, dt2 * sigma2, &x_noise, &x_diff_noise);
        *x += x_noise;
        *x_diff += x_diff_noise;
    }
}

void constant_velo_awgn_cov(real_t sigma, real_t cov[2][2], real_t *x, real_t *x_diff) {
    real_t x_noise, x_diff_noise;
    draw_gaussian_2d_cov(cov, &x_noise, &x_diff_noise);
    *x += x_noise * sigma;
    *x_diff += x_diff_noise * sigma;
}

real_t gaussian(real_t mu, real_t sigma2, real_t x) {
    real_t res = 1.0f / sqrtf(2.0f * (float)M_PI * sigma2) * expf(-(x-mu)*(x-mu)/(2.0f * sigma2));
    if (res <= FLT_EPSILON) {
        res = FLT_EPSILON;
    }
    return res;
}

