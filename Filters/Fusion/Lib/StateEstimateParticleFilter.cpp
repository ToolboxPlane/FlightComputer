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

StateEstimateParticleFilter::StateEstimateParticleFilter() {
    srand(time(nullptr)); // Required for the process noise
}

auto
StateEstimateParticleFilter::update(si::base::Second<> dt, const FlightControllerPackage &flightControllerPackage,
        const GpsMeasurement_t &gpsMeasurement, si::base::Meter<> distanceGround)
    -> system_state_t {
    if (particles.empty()) {
        init(1000, gpsMeasurement, distanceGround);
    }

    measurement_t measurement{};
    measurement.roll_angle = flightControllerPackage.roll;
    measurement.pitch_angle = flightControllerPackage.pitch;
    measurement.yaw_angle = flightControllerPackage.yaw;
    measurement.air_speed = 0; // @TODO
    measurement.ground_speed = static_cast<float>(gpsMeasurement.speed);
    measurement.vertical_speed = static_cast<float>(gpsMeasurement.climb);
    measurement.altitude_baro = 0; // @TODO navboard
    measurement.distance_ground = static_cast<float>(distanceGround);
    measurement.altitude_gps = static_cast<float>(gpsMeasurement.location.altitude);
    measurement.lat = static_cast<float>(gpsMeasurement.location.lat);
    measurement.lon = static_cast<float>(gpsMeasurement.location.lon);

    measurement_info_t measurementInfo{};
    measurementInfo.expected_error_lat = gpsMeasurement.epLat;
    measurementInfo.expected_error_lon = gpsMeasurement.epLon;
    measurementInfo.expected_error_vert = gpsMeasurement.epVert;
    measurementInfo.expected_error_speed = gpsMeasurement.epSpeed;
    measurementInfo.expected_error_climb = gpsMeasurement.epClimb;

    input_t input{};
    input.elevon_l = flightControllerPackage.elevonLeft;
    input.elevon_r = flightControllerPackage.elevonRight;
    input.motor = flightControllerPackage.motor;

    // Predict + Weight
    float weight_sum = 0;
    for (auto &particle : particles) {
        update_particle(&particle, &input, &measurement, static_cast<float>(dt), &measurementInfo);
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
        estimate.altitude += state.altitude * weight;
        estimate.altitude_above_ground += state.altitude_above_ground * weight;
        estimate.lat += state.lat * weight;
        estimate.lon += state.lon * weight;

        nEffInv += weight * weight;
    }

    // Resample
    std::vector<weighted_particle_t> newParticles;
    newParticles.resize(particles.size());

    resample(particles.data(), particles.size(), newParticles.data(), newParticles.size());

    particles = std::move(newParticles);

    return estimate;
}

void
StateEstimateParticleFilter::init(std::size_t numberOfParticles, const GpsMeasurement_t &gpsMeasurement,
        si::base::Meter<> distanceGround) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<real_t> altDist(0, 50);
    std::normal_distribution<real_t> altGroundDist(0, 0.5);
    std::normal_distribution<real_t> latLonDist(0, 0.0001);

    for (std::size_t c = 0; c < numberOfParticles; ++c) {
        system_state_t state{};
        state.altitude_above_ground = static_cast<real_t>(distanceGround) + altGroundDist(rng);
        state.altitude = 450 + altDist(rng);
        state.lon = gpsMeasurement.location.lat + latLonDist(rng);
        state.lat = gpsMeasurement.location.lon + latLonDist(rng);

        weighted_particle_t weightedParticle{};
        weightedParticle.x = state;
        weightedParticle.weight = 1.0f / numberOfParticles;
        particles.emplace_back(weightedParticle);
    }
}

