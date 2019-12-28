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

static constexpr auto INITIAL_PARTICLES = 100000;

StateEstimate::StateEstimate()
    : lastUpdate{getCurrSeconds()}, newParticleNum{INITIAL_PARTICLES} {
    srand(time(nullptr)); // Required for the process noise

    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_real_distribution<> dist{-180, 180};

    particles.reserve(INITIAL_PARTICLES);

    for (std::size_t c = 0; c < INITIAL_PARTICLES; ++c) {
        system_state_t state{};
        state.roll_angle = dist(gen);
        state.roll_rate = dist(gen);
        state.yaw_angle = dist(gen);
        state.yaw_rate = dist(gen);
        state.pitch_angle = dist(gen);
        state.pitch_rate = dist(gen);
        state.altitude_above_ground = 0;
        state.altitude = 0;
        state.lon = 0;
        state.lat = 0;

        weighted_particle_t weightedParticle{};
        weightedParticle.x = state;
        weightedParticle.weight = 1.0f / INITIAL_PARTICLES;
        particles.emplace_back(weightedParticle);
    }
}

auto
StateEstimate::update(const FlightControllerPackage &flightControllerPackage, const GpsMeasurement_t &gpsMeasurement)
    -> weighted_particle_t {

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

    const auto startTime = getCurrSeconds();
    const auto dt = startTime - lastUpdate;
    lastUpdate = startTime;

    // Predict + Weight
    update_cov_matrices(static_cast<float>(dt));
    float weight_sum = 0;
    for (auto &particle : particles) {
        update_particle(&particle, &input, &measurement, static_cast<float>(dt));
        weight_sum += particle.weight;
    }

    std::cout << "Predict: " << getCurrSeconds() - startTime << std::endl;

    weighted_particle_t likelihoodWinner{{}, -std::numeric_limits<float>::infinity()};

    // Fix PDF (-> Bayes * 1/p(z)) and determine most likely particle
    for (auto &[state, weight] : particles) {
        weight /= weight_sum;
        weight = std::min<real_t>(weight, 1.0);

        if (weight > likelihoodWinner.weight) {
            likelihoodWinner = {state, weight};
        }
    }

    std::cout << "Winner: " << getCurrSeconds() - startTime << std::endl;

    // Resample
    std::vector<weighted_particle_t> newParticles;
    newParticles.resize(newParticleNum);

    resample(particles.data(), particles.size(), newParticles.data(), newParticles.size());

    particles = std::move(newParticles);

    std::cout << "Resample: " << getCurrSeconds() - startTime << std::endl;

    // Controller for dynamic number of particles
    auto runtime = getCurrSeconds() - startTime;
    auto targetTime = dt * 0.8;
    if (runtime * 2 < targetTime) {
        newParticleNum *= 2;
    } else if (2 * targetTime < runtime) {
        newParticleNum /= 2;
    } else {
        auto factor = std::pow(static_cast<si::base::Second<>::type>(targetTime / runtime), 0.8);
        newParticleNum *= factor;
    }
    std::cout << "Particles: " << newParticleNum << std::endl;

    return likelihoodWinner;
}

auto StateEstimate::getCurrSeconds() -> si::base::Second<> {
    auto tp = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto microseconds = static_cast<long double>(
            std::chrono::duration_cast<std::chrono::microseconds>(tp).count());
    return microseconds / 10e6 * si::base::second;
}
