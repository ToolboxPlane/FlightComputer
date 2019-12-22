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

real_t normalize_angle(real_t angle) {
    int sign = 1;
    if (angle < 0) {
        sign = -1;
        angle = -angle;
    }

    while (angle > 180) { //@TODO not very efficient...
        angle -= 360;
    }

    return angle * (real_t)sign;
}

system_state_t predict(const system_state_t *x, const input_t *u, real_t dt, bool apply_noise) {
    system_state_t ret;
    ret.roll_angle = normalize_angle(x->roll_angle + dt * x->roll_rate);
    ret.roll_rate = x->roll_rate;
    ret.pitch_angle = normalize_angle(x->pitch_angle + dt * x->pitch_angle);
    ret.pitch_rate = x->pitch_rate;
    ret.yaw_angle = normalize_angle(x->yaw_angle + dt * x->yaw_rate);
    ret.yaw_rate = x->yaw_rate;
    ret.speed = x->speed;
    ret.altitude = x->altitude;
    ret.altitude_above_ground = x->altitude_above_ground;
    ret.lat = x->lat;
    ret.lon = x->lon;

    // @ TODO input

    if (apply_noise) {
        constant_velo_awgn(STDDEV_PROCESS_ROLL, dt, &ret.roll_angle, &ret.roll_rate);
        constant_velo_awgn(STDDEV_PROCESS_PITCH, dt, &ret.pitch_angle, &ret.pitch_rate);
        constant_velo_awgn(STDDEV_PROCESS_YAW, dt, &ret.yaw_angle, &ret.yaw_rate);
    }

    return ret;
}

measurement_t measure(const system_state_t *x) {
    measurement_t ret;
    ret.roll_angle = x->roll_angle;
    ret.roll_rate = x->roll_rate;
    ret.pitch_angle = x->pitch_angle;
    ret.pitch_rate = x->pitch_rate;
    ret.yaw_angle = x->yaw_angle;
    ret.yaw_rate = x->yaw_rate;
    ret.air_speed = x->speed;
    ret.ground_speed = x->speed;
    ret.altitude_baro = x->altitude;
    ret.distance_ground = x->altitude_above_ground; //@TODO roll and pitch
    ret.lat = x->lat;
    ret.lon = x->lon;

    return ret;
}

real_t likelihood(const measurement_t *measurement, const measurement_t *estimate) {
    real_t p_roll_angle = gaussian(measurement->roll_angle, VAR_MEASURE_ROLL_ANGLE, estimate->roll_angle);
    real_t p_roll_rate = gaussian(measurement->roll_rate, VAR_MEASURE_ROLL_RATE, estimate->roll_angle);
    real_t p_pitch_angle = gaussian(measurement->pitch_angle, VAR_MEASURE_PITCH_ANGLE, estimate->pitch_angle);
    real_t p_pitch_rate = gaussian(measurement->pitch_rate, VAR_MEASURE_PITCH_RATE, estimate->pitch_angle);
    real_t p_yaw_angle = gaussian(measurement->yaw_angle, VAR_MEASURE_YAW_ANGLE, estimate->yaw_angle);
    real_t p_yaw_rate = gaussian(measurement->yaw_rate, VAR_MEASURE_YAW_RATE, estimate->yaw_angle);

    return p_roll_angle * p_roll_rate * p_pitch_angle * p_pitch_rate * p_yaw_angle * p_yaw_rate;
}

void update_particle(weighted_particle_t *particle, const input_t *u, const measurement_t *z, real_t dt) {
    particle->x = predict(&particle->x, u, dt, true);
    measurement_t z_hat = measure(&particle->x);
    particle->weight *= likelihood(z, &z_hat);
}

void resample(const weighted_particle_t *old_particles, size_t num_old_particles, weighted_particle_t *new_particles,
              size_t num_new_particles) {
    for (size_t c=0; c<num_new_particles; ++c) { //@TODO parallelize
        real_t r = (real_t)rand() / RAND_MAX;
        real_t prob = 0;
        for (size_t p=0; p<num_old_particles; ++p) {
            prob += old_particles[p].weight;
            if (prob >= r) {
                new_particles[c].x = old_particles[p].x;
                new_particles[c].weight = 1.0f / num_new_particles;
                break;
            }
        }
    }
}
