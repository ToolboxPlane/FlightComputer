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
    float roll_angle; // Orientation and speed of the plane around the roll (forward) axis (degree)
    float pitch_angle; // Orientation and speed of the plane around the pitch (right) axis (degree)
    float yaw_angle; // Orientation and speed of the plane around the yaw (down) axis (degree)
    float speed; // The absolute speed of the plane
    float altitude; // The absolute altitude of the plane
    float altitude_above_ground; // The altitude above ground
    float lat, lon; // The global position of the plane
} system_state_t;

typedef struct {
    system_state_t x;
    float weight;
} weighted_particle_t;

typedef struct {
    float roll_angle; // Orientation and speed of the plane around the roll (forward) axis (degree)
    float pitch_angle; // Orientation and speed of the plane around the pitch (right) axis (degree)
    float yaw_angle; // Orientation and speed of the plane around the yaw (down) axis (degree)
    float air_speed; // The airspeed as measured by the pivot tube
    float ground_speed; // The ground speed of the plane
    float vertical_speed; // The vertical speed (climb) of the plane
    float altitude_baro; // The absolute altitude, as measured by the barometer
    float altitude_gps; // The absolute altitude, as measured by the gps
    float distance_ground; // The distance to the ground, as measured by the distance sensor
    float lat, lon; // The position, acquired by the GPS
} measurement_t;

typedef struct {
    float expected_error_lat, expected_error_lon, expected_error_vert, expected_error_speed, expected_error_climb;
} measurement_info_t;

typedef struct {
    float elevon_l, elevon_r;
    float motor;
} input_t;

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, float dt,
        const measurement_info_t *measurement_info);

void resample(const weighted_particle_t* old_particles, size_t num_old_particles,
              weighted_particle_t* new_particles, size_t num_new_particles);

#endif //FLIGHTCOMPUTER_SYSTEM_H
