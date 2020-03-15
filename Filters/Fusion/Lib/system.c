/**
 * @file system.c
 * @author paul
 * @date 21.12.19
 * @brief system @TODO
 */

#include "system.h"
#include "random_util.h"

#include <math.h>
#include <stdlib.h>

static real_t cv_trans_mat[2][2];

real_t normalize_angle(real_t angle) {
    fmodf(angle, 360);
    if (angle > 180) {
        angle -= 360;
    }

    return angle;
}

void update_cov_matrices(real_t dt) {
    real_t dt2 = dt * dt;
    real_t dt3 = dt2 * dt;
    real_t dt4 = dt2 * dt2;
    real_t cv_cov[2][2];
    cv_cov[0][0] = dt4 / 4;
    cv_cov[0][1] = cv_cov[1][0] = dt3 / 2 * 0.99f;
    cv_cov[1][1] = dt2;

    get_cov_trans_mat_2d(dt4/4, dt3/2 * 0.99f, dt2, cv_trans_mat);
}

system_state_t predict(const system_state_t *x, const input_t *u, real_t dt, bool apply_noise) {
    real_t vert_dist = sin(x->pitch_angle / 180 * M_PI) * x->speed * dt; // Vertical distance between in dt
    real_t horiz_dist = cos(x->pitch_angle / 180 * M_PI) * x->speed * dt; // Horizontal distance between in dt
    real_t lat_dist = horiz_dist * cos(x->yaw_angle / 180 * M_PI); // Distance north in dt (along latitude)
    real_t lon_dist = horiz_dist * sin(x->yaw_angle / 180 * M_PI); // Distance east in dt (along longitude)

    system_state_t ret;
    ret.speed = x->speed;
    ret.altitude = x->altitude + vert_dist;
    ret.altitude_above_ground = x->altitude_above_ground + vert_dist;
    ret.lat = x->lat + lat_dist * 360 / 40000.0;
    ret.lon = x->lon + lon_dist * 360 / 40000.0 * cos(x->lat / 180 * M_PI);

    // @ TODO input

    if (apply_noise) {
        //@TODO noise
        ret.altitude_above_ground += gaussian_box_muller(0, dt * 0.5);
    }

    return ret;
}

measurement_t measure(const system_state_t *x) {
    measurement_t ret;
    ret.roll_angle = x->roll_angle;
    ret.pitch_angle = x->pitch_angle;
    ret.yaw_angle = x->yaw_angle;
    ret.air_speed = x->speed;
    ret.ground_speed = x->speed * cos(x->pitch_angle / 180 * M_PI);
    ret.altitude_baro = x->altitude;
    ret.distance_ground = x->altitude_above_ground /
            (cos(x->roll_angle / 180 * M_PI) * cos(x->pitch_angle / 180 * M_PI));
    ret.lat = x->lat;
    ret.lon = x->lon;

    return ret;
}

real_t likelihood(const measurement_t *measurement, const measurement_t *estimate) {
    real_t p_distance_measure;
    if (estimate->distance_ground < 0) {
        p_distance_measure = 0;
    } else if (measurement->distance_ground < SRF02_MAX_DIST) {
        p_distance_measure = gaussian(measurement->distance_ground, VAR_SRF02, estimate->distance_ground);
    } else {
        p_distance_measure = 1 / (100.0 - SRF02_MAX_DIST);
    }

    // GPS: normal distribution with accuracy = 2 * sigma

    return p_distance_measure;
}

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, real_t dt) {
    particle->x.pitch_angle = z->pitch_angle;
    particle->x.roll_angle = z->roll_angle;
    particle->x.yaw_angle = z->yaw_angle;
    particle->x = predict(&particle->x, u, dt, true);
    measurement_t z_hat = measure(&particle->x);
    particle->weight *= likelihood(z, &z_hat);
}

void resample(const weighted_particle_t *old_particles, size_t num_old_particles, weighted_particle_t *new_particles,
              size_t num_new_particles) {
    real_t cdf[num_old_particles];
    real_t sum = 0;

    for (size_t c=0; c<num_old_particles; ++c) {
        sum += old_particles[c].weight;
        cdf[c] = sum;
    }

    for (size_t c=0; c<num_new_particles; ++c) { //@TODO parallelize
        real_t r = (real_t)RAND_IMPL() / RAND_IMPL_MAX;

        size_t lower_bound = 0;
        size_t upper_bound = num_old_particles - 1;
        while (lower_bound <= upper_bound) {
            size_t middle = lower_bound + (upper_bound - lower_bound) / 2;
            if (upper_bound - lower_bound < 2) {
                new_particles[c].x = old_particles[upper_bound].x;
                new_particles[c].weight = 1.0f / num_new_particles;
                break;
            } else if (cdf[middle] < r) {
                lower_bound = middle;
            } else {
                upper_bound = middle;
            }
        }
    }
}

