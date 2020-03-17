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

system_state_t predict(const system_state_t *x, const input_t *u, real_t dt, bool apply_noise) {
    real_t vert_dist = sin(x->pitch_angle / 180 * M_PI) * x->speed * dt; // Vertical distance between in dt
    real_t horiz_dist = cos(x->pitch_angle / 180 * M_PI) * x->speed * dt; // Horizontal distance between in dt
    real_t lat_dist = horiz_dist * cos(x->yaw_angle / 180 * M_PI); // Distance north in dt (along latitude)
    real_t lon_dist = horiz_dist * sin(x->yaw_angle / 180 * M_PI); // Distance east in dt (along longitude)

    system_state_t ret;
    ret.roll_angle = x->roll_angle;
    ret.pitch_angle = x->pitch_angle;
    ret.yaw_angle = x->yaw_angle;
    ret.speed = x->speed;
    ret.altitude = x->altitude + vert_dist;
    ret.altitude_above_ground = x->altitude_above_ground + vert_dist;
    ret.lat = x->lat + lat_dist * 360 / 40000.0;
    ret.lon = x->lon + lon_dist * 360 / 40000.0 * cos(x->lat / 180 * M_PI);

    // @ TODO input

    if (apply_noise) {
        /*
         * Noise gain:
         * For x = [altitude, lat, lon, v]^T
         * Gamma = [
         *      0.5 * dt^2 * sin(pitch);
         *      0.5 * dt^2 * cos(pitch) * cos(yaw);
         *      0.5 * dt^2 * cos(pitch) * sin(yaw);
         *      dt]
         */
        real_t gamma[4] = {
                0.5 * dt * dt * sin(x->pitch_angle / 180 * M_PI),
                0.5 * dt * dt * cos (x->pitch_angle / 180 * M_PI) * cos(x->yaw_angle / 180 * M_PI),
                0.5 * dt * dt * cos (x->pitch_angle / 180 * M_PI) * sin(x->yaw_angle / 180 * M_PI),
                dt
        };

        do {
            float noise = gaussian_box_muller(0, SIGMA_V);
            ret.altitude += noise * gamma[0];
            ret.altitude_above_ground += noise * gamma[0];
            ret.lat += noise * gamma[1];
            ret.lat += noise * gamma[2];
            ret.speed += noise * gamma[3];

            // Additional noise as ground is not static
            ret.altitude_above_ground += gaussian_box_muller(0, dt * SIGMA_GND);
        } while (ret.altitude_above_ground < 0);
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
    if (measurement->distance_ground > 0) {
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

    for (size_t c=0; c<num_new_particles; ++c) {
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

