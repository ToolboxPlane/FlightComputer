/**
 * @file config.h
 * @author paul
 * @date 22.12.19
 * @brief config @TODO
 */

#ifndef FLIGHTCOMPUTER_CONFIG_H
#define FLIGHTCOMPUTER_CONFIG_H

typedef double real_t;

#define STDDEV_PROCESS_ROLL 30
#define STDDEV_PROCESS_PITCH 50
#define STDDEV_PROCESS_YAW 8

#define VAR_MEASURE_ROLL_ANGLE 0.5
#define VAR_MEASURE_PITCH_ANGLE 0.5
#define VAR_MEASURE_YAW_ANGLE 0.5
#define VAR_MEASURE_ROLL_RATE 3.03
#define VAR_MEASURE_PITCH_RATE 1.12
#define VAR_MEASURE_YAW_RATE 0.95


#endif //FLIGHTCOMPUTER_CONFIG_H
