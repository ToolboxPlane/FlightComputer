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

#include "../../../constants.hpp"

#define VALUE_OR(var, val) (isnan(var)?val:var)

#define IEEE754_FIX_FACTOR 10000

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
    ret.lat = x->lat + lat_dist * 360 / EARTH_DIAMETER;
    ret.lon = x->lon + lon_dist * 360 / EARTH_DIAMETER / cos(x->lat / 180 * M_PI);

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

        float noise = gaussian_box_muller(0, SIGMA_V);
        ret.altitude += noise * gamma[0];
        ret.altitude_above_ground += noise * gamma[0];
        ret.lat += noise * gamma[1] * 360 / EARTH_DIAMETER;
        ret.lon += noise * gamma[2] * 360 / EARTH_DIAMETER / cos(ret.lat / 180 * M_PI);
        ret.speed += noise * gamma[3];

        // Additional noise as ground is not static
        ret.altitude_above_ground += gaussian_box_muller(0, dt * SIGMA_GND);
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
    ret.vertical_speed = x->speed * sin(x->pitch_angle / 180 * M_PI);
    ret.altitude_baro = x->altitude;
    ret.altitude_gps = x->altitude;
    ret.distance_ground = x->altitude_above_ground /
            (cos(x->roll_angle / 180 * M_PI) * cos(x->pitch_angle / 180 * M_PI));
    ret.lat = x->lat;
    ret.lon = x->lon;

    return ret;
}

real_t likelihood(const measurement_t *measurement, const measurement_t *estimate,
        const measurement_info_t *measurement_info) {
    real_t p_distance_measure;
    if (measurement->distance_ground > 0) {
        p_distance_measure = gaussian(measurement->distance_ground, VAR_SRF02, estimate->distance_ground);
    } else {
        p_distance_measure = 1 / (100.0 - SRF02_MAX_DIST);
    }

    // GPS: normal distribution with accuracy = 2 * sigma
    real_t sigma_lat = VALUE_OR(measurement_info->expected_error_lat, 10) / 2;
    real_t sigma_lon = VALUE_OR(measurement_info->expected_error_lon, 10) / 2;
    real_t sigma_vert = VALUE_OR(measurement_info->expected_error_vert, 10) / 2;
    real_t sigma_speed = VALUE_OR(measurement_info->expected_error_speed, 10) / 2;
    real_t sigma_climb = VALUE_OR(measurement_info->expected_error_climb, 10) / 2;


    real_t p_lat = 1, p_lon = 1, p_vert = 1, p_speed = 1, p_climb = 1;
    if (!isnan(measurement->lat) && !isnan(measurement->lon)) {
        real_t lat_dist = (measurement->lat - estimate->lat) / 360 * EARTH_DIAMETER;
        p_lat = gaussian(0, sigma_lat * sigma_lat, lat_dist);
        real_t lon_dist = (measurement->lon - estimate->lon) / 360 * EARTH_DIAMETER *
                          cos((measurement->lat + estimate->lat) / 2 / 180 * M_PI);
        p_lon = gaussian(0, sigma_lon * sigma_lon, lon_dist);
    }
    if (!isnan(measurement->altitude_gps)) {
        p_vert = gaussian(measurement->altitude_gps, sigma_vert * sigma_vert, estimate->altitude_gps);
    }
    if (!isnan(measurement->ground_speed)) {
        p_speed = gaussian(measurement->ground_speed, sigma_speed * sigma_speed, estimate->ground_speed);
    }
    if (!isnan(measurement->vertical_speed)) {
        p_climb = gaussian(measurement->vertical_speed, sigma_climb * sigma_climb, estimate->vertical_speed);
    }

    // @TODO Airspeed
    real_t p_airspeed = 1;

    // Barometer: Resolution, typical 0.3m;
    // Pressure noise: 19 PA RMS = sigma
    // @TODO Barometer
    real_t p_baro = 1;

    // These factors are not weights (in an non IEEE-754 world they would not change a thing),
    // they simply reduce numerical problems due to low likelihoods when they are multiplied together.
    return (p_distance_measure * IEEE754_FIX_FACTOR)
        * (p_lat * IEEE754_FIX_FACTOR)
        * (p_lon * IEEE754_FIX_FACTOR)
        * (p_vert * IEEE754_FIX_FACTOR)
        * (p_speed * IEEE754_FIX_FACTOR)
        * (p_climb * IEEE754_FIX_FACTOR)
        * (p_airspeed * IEEE754_FIX_FACTOR)
        * (p_baro * IEEE754_FIX_FACTOR);
}

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, real_t dt,
        const measurement_info_t *measurement_info) {
    particle->x.pitch_angle = z->pitch_angle;
    particle->x.roll_angle = z->roll_angle;
    particle->x.yaw_angle = z->yaw_angle;
    particle->x = predict(&particle->x, u, dt, true);
    measurement_t z_hat = measure(&particle->x);
    particle->weight *= likelihood(z, &z_hat, measurement_info);
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
        real_t r = (real_t)rand() / RAND_MAX;

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

