/**
 * @file system.h
 * @author paul
 * @date 21.12.19
 * @brief system @TODO
 */

#ifndef FLIGHTCOMPUTER_SYSTEM_H
#define FLIGHTCOMPUTER_SYSTEM_H

#include <stdbool.h>

typedef struct {
    float roll_angle, roll_rate;
    float pitch_angle, pitch_rate;
    float yaw_angle, yaw_rate;
    float speed;
    float altitude;
    float altitude_above_ground;
    float lat, lon;
} system_state_t;

typedef struct {
    float roll_angle, roll_rate;
    float pitch_angle, pitch_rate;
    float yaw_angle, yaw_rate;
    float air_speed;
    float ground_speed;
    float altitude;
    float distance_ground;
    float lat, lon;
} measurement_t;

typedef struct {
    float aileron_l, aileron_r;
    float vtail_l, vtail_r;
    float motor;
} input_t;

float update_particle(system_state_t *x, const input_t *u, const measurement_t *z, float dt);

#endif //FLIGHTCOMPUTER_SYSTEM_H
