/**
 * @file StateEstimate.cpp
 * @author paul
 * @date 21.12.19
 * @brief StateEstimate @TODO
 */

#include <cstdlib>
#include <ctime>
#include <random>
#include "StateEstimate.hpp"

StateEstimate::StateEstimate(std::size_t numberOfParticles)
    : lastUpdate{std::chrono::system_clock::now().time_since_epoch().count() * si::base::second} {
    srand(time(nullptr)); // Required for the process noise

    std::random_device rd{};
    std::mt19937 gen{rd()};

    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> gaussian{0, 1};

    particles.reserve(numberOfParticles);

    for (std::size_t c = 0; c < numberOfParticles; ++c) {
        system_state_t state{};
        state.roll_angle = gaussian(gen);
        state.roll_rate = gaussian(gen);
        state.yaw_angle = gaussian(gen);
        state.yaw_rate = gaussian(gen);
        state.pitch_angle = gaussian(gen);
        state.pitch_rate = gaussian(gen);
        state.altitude_above_ground = 0;
        state.altitude = 0;
        state.lon = 0;
        state.lat = 0;

        particles.emplace_back(std::make_pair(state, 1.0f));
    }
}

auto
StateEstimate::update(const FlightControllerPackage &flightControllerPackage, const GpsMeasurement_t &gpsMeasurement)
    -> WeightedParticle {

    measurement_t measurement{};
    measurement.roll_angle = flightControllerPackage.roll;
    measurement.roll_rate = static_cast<float>(flightControllerPackage.rollDeriv);
    measurement.pitch_angle = flightControllerPackage.pitch;
    measurement.pitch_rate = static_cast<float>(flightControllerPackage.pitchDeriv);
    measurement.yaw_angle = flightControllerPackage.yaw;
    measurement.yaw_rate = static_cast<float>(flightControllerPackage.yawDeriv);
    measurement.air_speed = 0; // @TODO
    measurement.ground_speed = static_cast<float>(gpsMeasurement.speed);
    measurement.altitude_baro = 0; // @TODO navboard
    measurement.distance_ground = 0; // @TODO navboard
    measurement.altitude_gps = static_cast<float>(gpsMeasurement.location.altitude);
    measurement.lat = static_cast<float>(gpsMeasurement.location.lat);
    measurement.lon = static_cast<float>(gpsMeasurement.location.lon);

    input_t input{};
    input.aileron_l = flightControllerPackage.aileronLeft;
    input.aileron_r = flightControllerPackage.aileronRight;
    input.vtail_l = flightControllerPackage.vtailLeft;
    input.vtail_r = flightControllerPackage.vtailRight;
    input.motor = flightControllerPackage.motor;

    const auto currTime = std::chrono::system_clock::now().time_since_epoch().count() * si::base::second;
    const auto dt = currTime - lastUpdate;
    lastUpdate = currTime;

    // Update step @TODO parallelize
    float weight_sum = 0;
    for (auto &[particle, weight] : particles) {
        weight *= update_particle(&particle, &input, &measurement, static_cast<float>(dt));
        weight_sum += weight;
    }

    // Fix PDF (-> Bayes * 1/p(z))
    for (auto &[particle, weight] : particles) {
        weight /= weight_sum;
    }
}
