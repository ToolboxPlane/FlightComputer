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

#include "config.h"
#include "../../../constants.hpp"

#include <stdio.h>

#define VALUE_OR(var, val) (isnan(var)?val:var)
#define DEG_TO_RAD(x) ((x) / 180.0F * (float)M_PI)
#define RAD_TO_DEG(x) ((x) * 180.0F * (float)M_PI)
#define DIST2LAT(y) ((y) * 360.0 / EARTH_CIRCUMFERENCE)
#define DIST2LON(x, lat) ((DIST2LAT((x))) / cos((lat) / 180.0 * M_PI))
#define LAT2DIST(y) ((float)((y) / 360.0 * EARTH_CIRCUMFERENCE))
#define LON2DIST(x, lat) ((float)((LAT2DIST((x))) * cos((lat) / 180.0 * M_PI)))

#define IEEE754_FIX_FACTOR 1000

system_state_t predict(const system_state_t *x, const input_t *u, float dt, bool apply_noise) {
    float pitch_offset = 0;
    float yaw_offset = 0;
    if (apply_noise) {
        pitch_offset = gaussian_box_muller(0, SIGMA_PITCH_OFFSET * dt);
        yaw_offset = gaussian_box_muller(0, SIGMA_YAW_OFFSET * dt);
    }

    float dist = x->speed * dt;
    float vert_dist =
            sinf(DEG_TO_RAD(x->pitch_angle + pitch_offset)) * dist; // Vertical distance between in dt
    float horiz_dist =
            cosf(DEG_TO_RAD(x->pitch_angle + pitch_offset)) * dist; // Horizontal distance between in dt
    float lat_dist = horiz_dist * cosf(DEG_TO_RAD(x->yaw_angle + yaw_offset)); // Distance north in dt (along latitude)
    float lon_dist = horiz_dist * sinf(DEG_TO_RAD(x->yaw_angle + yaw_offset)); // Distance east in dt (along longitude)

    system_state_t ret;
    ret.roll_angle = x->roll_angle;
    ret.roll_deriv = x->roll_deriv;
    ret.pitch_angle = x->pitch_angle;
    ret.pitch_deriv = x->pitch_deriv;
    ret.yaw_angle = x->yaw_angle;
    ret.yaw_deriv = x->yaw_deriv;
    ret.speed = x->speed;
    ret.altitude = x->altitude + vert_dist;
    ret.altitude_above_ground = x->altitude_above_ground + vert_dist;
    ret.lat = x->lat + DIST2LAT(lat_dist);
    ret.lon = x->lon + DIST2LON(lon_dist, x->lat);

    // @ TODO input

    if (apply_noise) {
        /*
         * Speed:
         * Noise gain:
         * For x = [altitude, lat, lon, v]^T
         * Gamma = [
         *      0.5 * dt^2 * sin(pitch);
         *      0.5 * dt^2 * cos(pitch) * cos(yaw);
         *      0.5 * dt^2 * cos(pitch) * sin(yaw);
         *      dt]
         */
        float gamma_speed[4] = {
                0.5F * dt * dt * sinf(DEG_TO_RAD(x->pitch_angle + pitch_offset)),
                0.5F * dt * dt * cosf(DEG_TO_RAD(x->pitch_angle + pitch_offset)) *
                    cosf(DEG_TO_RAD(x->yaw_angle + yaw_offset)),
                0.5F * dt * dt * cosf(DEG_TO_RAD(x->pitch_angle + pitch_offset)) *
                    sinf(DEG_TO_RAD(x->yaw_angle + yaw_offset)),
                dt
        };

        float noise_speed = gaussian_box_muller(0, SIGMA_V);
        ret.altitude += noise_speed * gamma_speed[0];
        ret.altitude_above_ground += noise_speed * gamma_speed[0];
        ret.lat += DIST2LAT(noise_speed * gamma_speed[1]);
        ret.lon += DIST2LON(noise_speed * gamma_speed[2], ret.lat);
        ret.speed += noise_speed * gamma_speed[3];

        // Additional noise as ground is not static
        ret.altitude_above_ground += gaussian_box_muller(0, dt * SIGMA_GND + gamma_speed[0] * SIGMA_V);
    }

    return ret;
}

measurement_t measure(const system_state_t *x) {
    measurement_t ret;
    ret.roll_angle = (float) ((int) (x->roll_angle * 2.0F + 0.5F)) / 2.0F;
    ret.roll_deriv = x->roll_deriv;
    ret.pitch_angle = (float) ((int) (x->pitch_angle * 2.0F + 0.5F)) / 2.0F;
    ret.pitch_deriv = x->pitch_deriv;
    ret.yaw_angle = (float) ((int) (x->yaw_angle * 2.0F + 0.5F)) / 2.0F;
    ret.yaw_deriv = x->yaw_deriv;
    ret.air_speed = x->speed;
    ret.ground_speed = x->speed * cosf(DEG_TO_RAD(x->pitch_angle));
    ret.vertical_speed = x->speed * sinf(DEG_TO_RAD(x->pitch_angle));
    ret.altitude_baro = (float) (int) (x->altitude + 0.5F); // Measurement resolution is 1 meter
    ret.altitude_gps = x->altitude;
    float distance_ground = x->altitude_above_ground /
                            (cosf(DEG_TO_RAD(x->roll_angle)) * cosf(DEG_TO_RAD(x->pitch_angle)));
    ret.distance_ground = (float) ((int) (distance_ground * 100.0F + 0.5F)) / 100.0F; // Measuremnt resolution is 1cm
    ret.lat = x->lat;
    ret.lon = x->lon;

    return ret;
}

float likelihood(const measurement_t *measurement, const measurement_t *estimate,
                 const measurement_info_t *measurement_info) {
    // US
    float p_distance_measure;
    if (estimate->distance_ground < 0) {
        p_distance_measure = 0;
    } else if (measurement->distance_ground > 0) {
        p_distance_measure = gaussian(measurement->distance_ground, SIGMA_SRF02, estimate->distance_ground);
    } else { // No distance measurement
        if (estimate->distance_ground < SRF02_MAX_DIST) { // High altitudes are more likely
            p_distance_measure = 0.5F;
        } else {
            p_distance_measure = 1;
        }
    }

    // GPS: normal distribution with accuracy = 2 * sigma
    float sigma_lat_dist = VALUE_OR(measurement_info->expected_error_lat, 30);
    float sigma_lon_dist = VALUE_OR(measurement_info->expected_error_lon, 30);
    float sigma_vert = VALUE_OR(measurement_info->expected_error_vert, 50);

    float sigma_speed = VALUE_OR(measurement_info->expected_error_speed, 3);
    float sigma_climb = VALUE_OR(measurement_info->expected_error_climb, 20);

    float p_lat = 1, p_lon = 1, p_vert = 1, p_speed = 1, p_climb = 1;
    if (!isnan(measurement->lat) && !isnan(measurement->lon)) {
        float lat_dist = LAT2DIST(measurement->lat - estimate->lat);
        p_lat = gaussian(0, sigma_lat_dist, lat_dist);
        float lon_dist = LON2DIST(measurement->lon - estimate->lon, (measurement->lat + estimate->lat) / 2);
        p_lon = gaussian(0, sigma_lon_dist, lon_dist);
    }
    if (!isnan(measurement->altitude_gps)) {
        p_vert = gaussian(measurement->altitude_gps, sigma_vert, estimate->altitude_gps);
    }
    if (!isnan(measurement->ground_speed)) {
        p_speed = gaussian(measurement->ground_speed, sigma_speed, estimate->ground_speed);
        //printf("Meas: %f\tEst: %f\tP: %f\n", measurement->ground_speed, estimate->ground_speed, p_speed);
    }
    if (!isnan(measurement->vertical_speed)) {
        p_climb = gaussian(measurement->vertical_speed, sigma_climb, estimate->vertical_speed);
    }

    if (estimate->ground_speed < 0) {
        p_speed = 0;
    }

    // @TODO Airspeed
    float p_airspeed = 1;

    // Barometer
    float p_baro = gaussian(measurement->altitude_baro, SIGMA_BARO + measurement_info->additional_baro_uncertainty,
                            estimate->altitude_baro);

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

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, float dt,
                     const measurement_info_t *measurement_info) {
    particle->x.roll_angle = z->roll_angle;
    particle->x.pitch_angle = z->pitch_angle;
    particle->x.yaw_angle = z->yaw_angle;
    particle->x = predict(&particle->x, u, dt, true);
    measurement_t z_hat = measure(&particle->x);
    particle->weight *= likelihood(z, &z_hat, measurement_info);
}

void resample(const weighted_particle_t *old_particles, size_t num_old_particles, weighted_particle_t *new_particles,
              size_t num_new_particles) {
    float cdf[num_old_particles];
    float sum = 0;

    for (size_t c = 0; c < num_old_particles; ++c) {
        sum += old_particles[c].weight;
        cdf[c] = sum;
    }

    for (size_t c = 0; c < num_new_particles; ++c) {
        float r = (float) rand() / RAND_MAX;

        size_t lower_bound = 0;
        size_t upper_bound = num_old_particles - 1;
        while (lower_bound <= upper_bound) {
            size_t middle = lower_bound + (upper_bound - lower_bound) / 2;
            if (upper_bound - lower_bound < 2) {
                new_particles[c].x = old_particles[upper_bound].x;
                new_particles[c].weight = 1.0F / num_new_particles;
                break;
            } else if (cdf[middle] < r) {
                lower_bound = middle;
            } else {
                upper_bound = middle;
            }
        }
    }
}

