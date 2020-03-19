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
    real_t roll_angle; // Orientation and speed of the plane around the roll (forward) axis (degree)
    real_t pitch_angle; // Orientation and speed of the plane around the pitch (right) axis (degree)
    real_t yaw_angle; // Orientation and speed of the plane around the yaw (down) axis (degree)
    real_t speed; // The absolute speed of the plane
    real_t altitude; // The absolute altitude of the plane
    real_t altitude_above_ground; // The altitude above ground
    real_t lat, lon; // The global position of the plane
} system_state_t;

typedef struct {
    system_state_t x;
    real_t weight;
} weighted_particle_t;

typedef struct {
    real_t roll_angle; // Orientation and speed of the plane around the roll (forward) axis (degree)
    real_t pitch_angle; // Orientation and speed of the plane around the pitch (right) axis (degree)
    real_t yaw_angle; // Orientation and speed of the plane around the yaw (down) axis (degree)
    real_t air_speed; // The airspeed as measured by the pivot tube
    real_t ground_speed; // The ground speed of the plane
    real_t altitude_baro; // The absolute altitude, as measured by the barometer
    real_t altitude_gps; // The absolute altitude, as measured by the gps
    real_t distance_ground; // The distance to the ground, as measured by the distance sensor
    real_t lat, lon; // The position, acquired by the GPS
} measurement_t;

typedef struct {
    real_t elevon_l, elevon_r;
    real_t motor;
} input_t;

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, real_t dt);

void resample(const weighted_particle_t* old_particles, size_t num_old_particles,
              weighted_particle_t* new_particles, size_t num_new_particles);

#endif //FLIGHTCOMPUTER_SYSTEM_H
