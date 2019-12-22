/**
 * @file system.c
 * @author paul
 * @date 21.12.19
 * @brief system @TODO
 */

#include "system.h"
#include "random_util.h"

#include <math.h>

#define STDDEV_PROCESS_ROLL 5
#define STDDEV_PROCESS_PITCH 5
#define STDDEV_PROCESS_YAW 5

#define VAR_MEASURE_ROLL_ANGLE 5
#define VAR_MEASURE_PITCH_ANGLE 5
#define VAR_MEASURE_YAW_ANGLE 5
#define VAR_MEASURE_ROLL_RATE 5
#define VAR_MEASURE_PITCH_RATE 5
#define VAR_MEASURE_YAW_RATE 5

float normalize_angle(float angle) {
    int sign = 1;
    if (angle < 0) {
        sign = -1;
        angle = -angle;
    }

    while (angle > 180) {
        angle -= 360;
    }

    return angle * (float)sign;
}

system_state_t predict(const system_state_t *x, const input_t *u, float dt, bool apply_noise) {
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

float likelihood(const measurement_t *measurement, const measurement_t *estimate) {
    float p_roll_angle = gaussian(measurement->roll_angle, VAR_MEASURE_ROLL_ANGLE, estimate->roll_angle);
    float p_roll_rate = gaussian(measurement->roll_rate, VAR_MEASURE_ROLL_RATE, estimate->roll_angle);
    float p_pitch_angle = gaussian(measurement->pitch_angle, VAR_MEASURE_PITCH_ANGLE, estimate->pitch_angle);
    float p_pitch_rate = gaussian(measurement->pitch_rate, VAR_MEASURE_PITCH_RATE, estimate->pitch_angle);
    float p_yaw_angle = gaussian(measurement->yaw_angle, VAR_MEASURE_YAW_ANGLE, estimate->yaw_angle);
    float p_yaw_rate = gaussian(measurement->yaw_rate, VAR_MEASURE_YAW_RATE, estimate->yaw_angle);

    return p_roll_angle * p_roll_rate * p_pitch_angle * p_pitch_rate * p_yaw_angle * p_yaw_rate;
}

float update_particle(system_state_t *x, const input_t *u, const measurement_t *z, float dt) {
    *x = predict(x, u, dt, true);
    measurement_t z_hat = measure(x);
    return likelihood(z, &z_hat);
}
