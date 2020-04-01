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
        const GpsMeasurement_t &gpsMeasurement, const NavPackage &navPackage)
    -> system_state_t {
    if (particles.empty()) {
        init(10000, gpsMeasurement, navPackage.usDistance);
    }

    measurement_t measurement{};
    measurement.roll_angle = flightControllerPackage.roll;
    measurement.pitch_angle = flightControllerPackage.pitch;
    measurement.yaw_angle = flightControllerPackage.yaw;
    measurement.air_speed = 0; // @TODO
    measurement.ground_speed = static_cast<float>(gpsMeasurement.speed);
    measurement.vertical_speed = static_cast<float>(gpsMeasurement.climb);
    measurement.altitude_baro = static_cast<float>(navPackage.baroAltitude);
    measurement.distance_ground = static_cast<float>(navPackage.usDistance);
    measurement.altitude_gps = static_cast<float>(gpsMeasurement.location.altitude);
    measurement.lat = static_cast<float>(gpsMeasurement.location.lat);
    measurement.lon = static_cast<float>(gpsMeasurement.location.lon);

    measurement_info_t measurementInfo{};
    measurementInfo.expected_error_lat = static_cast<float>(gpsMeasurement.epLat);
    measurementInfo.expected_error_lon = static_cast<float>(gpsMeasurement.epLon);
    measurementInfo.expected_error_vert = static_cast<float>(gpsMeasurement.epVert);
    measurementInfo.expected_error_speed = static_cast<float>(gpsMeasurement.epSpeed);
    measurementInfo.expected_error_climb = static_cast<float>(gpsMeasurement.epClimb);

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

    if (weight_sum < std::numeric_limits<float>::epsilon()) {
        std::cerr << "weight_sum = 0" << std::endl;
    }
    if(std::isnan(weight_sum)) {
        std::cerr << "weight_sum = NaN" << std::endl;
    }

    system_state_t estimate{};

    float nEffInv = 0;

    // Fix PDF (-> Bayes * 1/p(z)) and determine most likely particle
    for (auto &[state, weight] : particles) {
        weight /= weight_sum;
        weight = std::min(weight, 1.0F);

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
    std::normal_distribution<float> altDist(0, 10);
    std::normal_distribution<float> altGroundDist(0, 0.1);
    std::normal_distribution<float> latLonDist(0, 0.00001);

    for (std::size_t c = 0; c < numberOfParticles; ++c) {
        weighted_particle_t weightedParticle{};
        weightedParticle.x.altitude_above_ground = static_cast<float>(distanceGround) + altGroundDist(rng);
        weightedParticle.x.altitude = static_cast<float>(gpsMeasurement.location.altitude) + altDist(rng);
        weightedParticle.x.lon = static_cast<float>(gpsMeasurement.location.lat) + latLonDist(rng);
        weightedParticle.x.lat = static_cast<float>(gpsMeasurement.location.lon) + latLonDist(rng);
        weightedParticle.x.speed = 0;

        weightedParticle.weight = 1.0F / numberOfParticles;
        particles.emplace_back(weightedParticle);
    }
}

