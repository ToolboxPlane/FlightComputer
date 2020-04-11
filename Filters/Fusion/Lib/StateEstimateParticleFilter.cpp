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

constexpr auto NUM_PARTICLES = 300000;

StateEstimateParticleFilter::StateEstimateParticleFilter() {
    srand(time(nullptr)); // Required for the process noise
}

auto
StateEstimateParticleFilter::update(si::Second<> dt, const FlightControllerPackage &flightControllerPackage,
                                    const GpsMeasurement_t &gpsMeasurement, const NavPackage &navPackage,
                                    si::Meter<> additionalBaroUncertainty)
-> system_state_t {
    if (particles.empty()) {
        init(NUM_PARTICLES, flightControllerPackage, gpsMeasurement, navPackage);
    }

    measurement_t measurement{};
    measurement.roll_angle = flightControllerPackage.roll;
    measurement.roll_deriv = static_cast<float>(flightControllerPackage.rollDeriv);
    measurement.pitch_angle = flightControllerPackage.pitch;
    measurement.pitch_deriv = static_cast<float>(flightControllerPackage.pitchDeriv);
    measurement.yaw_angle = flightControllerPackage.yaw;
    measurement.yaw_deriv = static_cast<float>(flightControllerPackage.yawDeriv);
    measurement.air_speed = 0; // @TODO
    measurement.ground_speed = static_cast<float>(gpsMeasurement.speed);
    measurement.vertical_speed = static_cast<float>(gpsMeasurement.climb);
    measurement.altitude_baro = static_cast<float>(navPackage.baroAltitude);
    measurement.distance_ground = static_cast<float>(navPackage.usDistance);
    measurement.altitude_gps = static_cast<float>(gpsMeasurement.location.altitude);
    measurement.lat = gpsMeasurement.location.lat;
    measurement.lon = gpsMeasurement.location.lon;

    measurement_info_t measurementInfo{};
    measurementInfo.expected_error_lat = static_cast<float>(gpsMeasurement.epLat);
    measurementInfo.expected_error_lon = static_cast<float>(gpsMeasurement.epLon);
    measurementInfo.expected_error_vert = static_cast<float>(gpsMeasurement.epVert);
    measurementInfo.expected_error_speed = static_cast<float>(gpsMeasurement.epSpeed);
    measurementInfo.expected_error_climb = static_cast<float>(gpsMeasurement.epClimb);
    measurementInfo.additional_baro_uncertainty = static_cast<float>(additionalBaroUncertainty);

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
        init(NUM_PARTICLES, flightControllerPackage, gpsMeasurement, navPackage);
        weight_sum = 1;
    }
    if (std::isnan(weight_sum)) {
        std::cerr << "weight_sum = NaN" << std::endl;
        init(NUM_PARTICLES, flightControllerPackage, gpsMeasurement, navPackage);
        weight_sum = 1;
    }

    system_state_t estimate{};
    system_state_t kahanComp{};

    float nEffInv = 0;

    // Fix PDF (-> Bayes * 1/p(z)) and determine expected state E{x}
    for (auto &[state, weight] : particles) {
        weight /= weight_sum;
        kahanSum(estimate.roll_angle, kahanComp.roll_angle, state.roll_angle * weight);
        kahanSum(estimate.roll_deriv, kahanComp.roll_deriv, state.roll_deriv * weight);
        kahanSum(estimate.pitch_angle, kahanComp.pitch_angle, state.pitch_angle * weight);
        kahanSum(estimate.pitch_deriv, kahanComp.pitch_deriv, state.pitch_deriv * weight);
        kahanSum(estimate.yaw_angle, kahanComp.yaw_angle, state.yaw_angle * weight);
        kahanSum(estimate.yaw_deriv, kahanComp.yaw_deriv, state.yaw_deriv * weight);
        kahanSum(estimate.speed, kahanComp.speed, state.speed * weight);
        kahanSum(estimate.altitude, kahanComp.altitude, state.altitude * weight);
        kahanSum(estimate.altitude_above_ground,  kahanComp.altitude_above_ground, state.altitude_above_ground * weight);
        kahanSum(estimate.lat, kahanComp.lat, (state.lat - particles.front().x.lat) * weight);
        kahanSum(estimate.lon, kahanComp.lon, (state.lon - particles.front().x.lon) * weight);

        nEffInv += weight * weight;
    }

    estimate.lat += particles.front().x.lat;
    estimate.lon += particles.front().x.lon;

    if (estimate.speed < 0) {
        estimate.speed = 0;
    }

    if (estimate.altitude_above_ground < 0) {
        estimate.altitude_above_ground = 0;
    }

    // Resample
    std::vector<weighted_particle_t> newParticles;
    newParticles.resize(particles.size());

    resample(particles.data(), particles.size(), newParticles.data(), newParticles.size());

    particles = std::move(newParticles);

    return estimate;
}

void
StateEstimateParticleFilter::init(std::size_t numberOfParticles,
                                  const FlightControllerPackage &flightControllerPackage,
                                  const GpsMeasurement_t &gpsMeasurement,
                                  const NavPackage &navPackage) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<float> altDist(0, 1);
    std::normal_distribution<float> altGroundDist(0, 0.1);
    std::normal_distribution<float> latLonDist(0, 0.00001);
    std::normal_distribution<float> speedDist(0, 5);
    particles.clear();

    for (std::size_t c = 0; c < numberOfParticles; ++c) {
        weighted_particle_t weightedParticle{};
        weightedParticle.x.pitch_angle = static_cast<float>(flightControllerPackage.pitch);
        weightedParticle.x.pitch_deriv = 0;
        weightedParticle.x.roll_angle = static_cast<float>(flightControllerPackage.roll);
        weightedParticle.x.roll_deriv = 0;
        weightedParticle.x.yaw_angle = static_cast<float>(flightControllerPackage.yaw);
        weightedParticle.x.yaw_deriv = 0;
        weightedParticle.x.altitude_above_ground = static_cast<float>(navPackage.usDistance) + altGroundDist(rng);
        weightedParticle.x.altitude = static_cast<float>(gpsMeasurement.location.altitude) + altDist(rng);
        weightedParticle.x.lat = static_cast<float>(gpsMeasurement.location.lat) + latLonDist(rng);
        weightedParticle.x.lon = static_cast<float>(gpsMeasurement.location.lon) + latLonDist(rng);
        if (std::isnan(static_cast<si::default_type>(gpsMeasurement.speed))) {
            weightedParticle.x.speed = 0;
        } else {
            weightedParticle.x.speed = static_cast<float>(gpsMeasurement.speed);
        }

        weightedParticle.weight = 1.0F / numberOfParticles;
        particles.emplace_back(weightedParticle);
    }
}

template<typename T>
void StateEstimateParticleFilter::kahanSum(T &sum, T &comp, T toAdd) {
    auto y = toAdd - comp;
    auto t = sum + y;
    comp = (t - sum) - y;
    sum = t;
}

