/**
 * @file config.h
 * @author paul
 * @date 22.12.19
 * @brief config @TODO
 */

#ifndef FLIGHTCOMPUTER_CONFIG_H
#define FLIGHTCOMPUTER_CONFIG_H

// Process
#define SIGMA_GND 1.0F
#define SIGMA_V 5.0F
#define SIGMA_PITCH_OFFSET 30.0F
#define SIGMA_YAW_OFFSET 90.0F

// Measurement
#define SIGMA_SRF02 0.005F
#define SRF02_MAX_DIST 5.0F
#define SIGMA_BARO 1.0F
#define SIGMA_BNO_ANGLE 45.0F
#define SIGMA_BNO_RATE 2.55F

#endif // FLIGHTCOMPUTER_CONFIG_H
