/**
 * @file system.c
 * @author paul
 * @date 21.12.19
 * @brief system @TODO
 */

#include "system.h"
#include "random_util.h"

#define STDDEV_ROLL 5
#define STDDEV_PITCH 5
#define STDDEV_YAW 5

system_state_t predict(const system_state_t *x, const input_t *u, float dt, bool apply_noise) {
    system_state_t ret;
    ret.roll_angle = x->roll_angle + dt * x->roll_rate;
    ret.roll_rate = x->roll_rate;
    ret.pitch_angle = x->pitch_angle + dt * x->pitch_angle;
    ret.pitch_rate = x->pitch_rate;
    ret.yaw_angle = x->yaw_angle + dt * x->yaw_rate;
    ret.yaw_rate = x->yaw_rate;
    ret.speed = x->speed;
    ret.altitude = x->altitude;
    ret.altitude_above_ground = x->altitude_above_ground;
    ret.lat = x->lat;
    ret.lon = x->lon;

    // @ TODO input

    if (apply_noise) {
        constant_velo_awgn(STDDEV_ROLL, dt, &ret.roll_angle, &ret.roll_rate);
        constant_velo_awgn(STDDEV_PITCH, dt, &ret.pitch_angle, &ret.pitch_rate);
        constant_velo_awgn(STDDEV_YAW, dt, &ret.yaw_angle, &ret.yaw_rate);
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
    ret.altitude = x->altitude;
    ret.distance_ground = x->altitude_above_ground; //@TODO roll and pitch
    ret.lat = x->lat;
    ret.lon = x->lon;

    return ret;
}

float likelihood(const measurement_t *z, const measurement_t *z_hat) {
    return 0;
}

float update_particle(system_state_t *x, const input_t *u, const measurement_t *z, float dt) {
    *x = predict(x, u, dt, true);
    measurement_t z_hat = measure(x);
    return likelihood(z, &z_hat);
}
