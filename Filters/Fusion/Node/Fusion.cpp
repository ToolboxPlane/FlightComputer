//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"
#include "../Lib/DecodePackage.hpp"

#ifdef DEBUG
    constexpr auto NUM_PARTICLES = 100;
#else
    constexpr auto NUM_PARTICLES = 20000;
#endif

namespace filter {
    using namespace si::extended;
    using namespace si::literals;

    Fusion::Fusion() : particleFilter{NUM_PARTICLES} {
        this->start();
    }

    OutputChannel<State_t> &Fusion::getChannelOut() {
        return out;
    }

    InputChannel<GpsMeasurement_t> &Fusion::getGpsIn() {
        return gpsIn;
    }

    InputChannel<rcLib::Package> &Fusion::getFlightControllerIn() {
        return flightControllerIn;
    }

    InputChannel<rcLib::Package> &Fusion::getBaseIn() {
        return baseIn;
    }

    InputChannel<rcLib::Package> &Fusion::getRemoteIn() {
        return remoteIn;
    }

    InputChannel<rcLib::Package> &Fusion::getPdbIn() {
        return pdbIn;
    }

    InputChannel<rcLib::Package> &Fusion::getTaranisIn() {
        return taranisIn;
    }

    void Fusion::run() {
        while (!flightControllerIn.isClosed()) {
            gpsIn.get(lastGpsMeasurement);
            pdbIn.get(lastPdbPackage);
            taranisIn.get(lastTaranisPackage);
            baseIn.get(lastBasePackage);
            remoteIn.get(lastRemotePackage);

            if (flightControllerIn.get(lastFcPackage)) {
                process();
            } else {
                std::this_thread::yield();
            }
        }
    }

    void Fusion::process() {
        State_t res{};

        if (lastPdbPackage.has_value()) {
            res.pdbPackage = fusion::decodePackage<PdbPackage>(lastPdbPackage.value());
        }

        if (lastTaranisPackage.has_value()) {
            res.taranisPackage = fusion::decodePackage<TaranisPackage>(lastTaranisPackage.value());
        }

        if (lastRemotePackage.has_value()) {
            res.loraRemote = fusion::decodePackage<LoraPackage>(lastRemotePackage.value());
        }

        if (lastGpsMeasurement.has_value() && lastGpsMeasurement.value().fixAquired) {
            auto flightControllerData = fusion::decodePackage<FlightControllerPackage>(lastFcPackage);
            auto start = StateEstimate::getCurrSeconds();
            auto [state, likelihood] = particleFilter.update(flightControllerData, lastGpsMeasurement.value());
            auto dur = StateEstimate::getCurrSeconds() - start;
            std::cout << "PF (" << NUM_PARTICLES << " Particles): " << dur << std::endl;

            res.roll = state.roll_angle;
            res.rollDiff = state.roll_rate * hertz;
            res.pitch = state.pitch_angle;
            res.pitchDiff = state.pitch_rate * hertz;
            res.yaw = state.yaw_angle;
            res.yawDiff = state.yaw_rate * hertz;
            res.speed = state.speed * speed;
            res.altitude = state.altitude * meter;
            res.altitudeAboveGround = state.altitude_above_ground * meter;
            res.lat = state.lat;
            res.lon = state.lon;

            out.put(res);
        }
    }
}
