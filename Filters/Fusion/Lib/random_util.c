#include "random_util.h"

#include <math.h>
#include <stdlib.h>
#include <values.h>

float gaussian_box_muller(float mu, float sigma) {
    static float v, fac;
    static int phase = 0;
    float S, Z, U1, U2, u;

    if (phase) {
        Z = v * fac;
    } else {
        do {
            U1 = (float) rand() / RAND_MAX;
            U2 = (float) rand() / RAND_MAX;

            u = 2.f * U1 - 1.f;
            v = 2.f * U2 - 1.f;
            S = u * u + v * v;
        } while (S >= 1);

        fac = sqrtf(-2.f * logf(S) / S);
        Z = u * fac;
    }

    phase = 1 - phase;

    return Z * sigma + mu;
}

float gaussian(float mu, float sigma, float x) {
    float res = 1.0f / (sqrtf(2.0f * (float) M_PI) * sigma) * expf(-(x - mu) * (x - mu) / (2.0f * sigma * sigma));
    if (res <= FLT_EPSILON) {
        res = FLT_EPSILON;
    }
    return res;
}
