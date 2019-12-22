/**
 * @file system.h
 * @author paul
 * @date 21.12.19
 * @brief system @TODO
 */

#ifndef FLIGHTCOMPUTER_SYSTEM_H
#define FLIGHTCOMPUTER_SYSTEM_H

#include <stdbool.h>
#include <glob.h>
#include "config.h"

typedef struct {
    real_t roll_angle, roll_rate;
    real_t pitch_angle, pitch_rate;
    real_t yaw_angle, yaw_rate;
    real_t speed;
    real_t altitude;
    real_t altitude_above_ground;
    real_t lat, lon;
} system_state_t;

typedef struct {
    system_state_t x;
    real_t weight;
} weighted_particle_t;

typedef struct {
    real_t roll_angle, roll_rate;
    real_t pitch_angle, pitch_rate;
    real_t yaw_angle, yaw_rate;
    real_t air_speed;
    real_t ground_speed;
    real_t altitude_baro;
    real_t altitude_gps;
    real_t distance_ground;
    real_t lat, lon;
} measurement_t;

typedef struct {
    real_t aileron_l, aileron_r;
    real_t vtail_l, vtail_r;
    real_t motor;
} input_t;

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, real_t dt);

void resample(const weighted_particle_t* old_particles, size_t num_old_particles,
              weighted_particle_t* new_particles, size_t num_new_particles);

#endif //FLIGHTCOMPUTER_SYSTEM_H
