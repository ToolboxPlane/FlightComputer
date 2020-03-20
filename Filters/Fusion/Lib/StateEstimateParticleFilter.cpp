/**
 * @file StateEstimateParticleFilter.cpp
 * @author paul
 * @date 21.12.19
 * @brief StateEstimateParticleFilter @TODO
 */

#include <cstdlib>
#include <ctime>
#include <random>
#include "StateEstimateParticleFilter.hpp"

static constexpr auto PARTICLES = 10000;

StateEstimateParticleFilter::StateEstimateParticleFilter() {
    srand(time(nullptr)); // Required for the process noise

    particles.reserve(PARTICLES);

    for (std::size_t c = 0; c < PARTICLES; ++c) {
        system_state_t state{};
        state.altitude_above_ground = 0.0;
        state.altitude = 0;
        state.lon = 0;
        state.lat = 0;

        weighted_particle_t weightedParticle{};
        weightedParticle.x = state;
        weightedParticle.weight = 1.0f / PARTICLES;
        particles.emplace_back(weightedParticle);
    }
}

auto
StateEstimateParticleFilter::update(si::base::Second<> dt, const FlightControllerPackage &flightControllerPackage,
        const GpsMeasurement_t &gpsMeasurement, si::base::Meter<> distanceGround)
    -> system_state_t {

    measurement_t measurement{};
    measurement.roll_angle = flightControllerPackage.roll;
    measurement.pitch_angle = flightControllerPackage.pitch;
    measurement.yaw_angle = flightControllerPackage.yaw;
    measurement.air_speed = 0; // @TODO
    measurement.ground_speed = static_cast<float>(gpsMeasurement.speed);
    measurement.altitude_baro = 0; // @TODO navboard
    measurement.distance_ground = static_cast<float>(distanceGround);
    measurement.altitude_gps = static_cast<float>(gpsMeasurement.location.altitude);
    measurement.lat = static_cast<float>(gpsMeasurement.location.lat);
    measurement.lon = static_cast<float>(gpsMeasurement.location.lon);

    input_t input{};
    input.elevon_l = flightControllerPackage.elevonLeft;
    input.elevon_r = flightControllerPackage.elevonRight;
    input.motor = flightControllerPackage.motor;

    // Predict + Weight
    float weight_sum = 0;
    for (auto &particle : particles) {
        update_particle(&particle, &input, &measurement, static_cast<float>(dt));
        weight_sum += particle.weight;
    }

    system_state_t estimate{};

    double nEffInv = 0;

    // Fix PDF (-> Bayes * 1/p(z)) and determine most likely particle
    for (auto &[state, weight] : particles) {
        weight /= weight_sum;
        weight = std::min<real_t>(weight, 1.0);

        //std::cout << "(" << state.altitude_above_ground << ", " << estimate.speed << "):\t" << weight << "\n";

        estimate.roll_angle += state.roll_angle * weight;
        estimate.pitch_angle += state.pitch_angle * weight;
        estimate.yaw_angle += state.yaw_angle * weight;
        estimate.speed += state.speed * weight;
        estimate.altitude += state.altitude;
        estimate.altitude_above_ground += state.altitude_above_ground * weight;
        estimate.lat += state.lat * weight;
        estimate.lon += state.lon * weight;

        nEffInv += weight * weight;
    }

    // Resample
    std::vector<weighted_particle_t> newParticles;
    newParticles.resize(PARTICLES);

    resample(particles.data(), particles.size(), newParticles.data(), newParticles.size());

    particles = std::move(newParticles);

    return estimate;
}

