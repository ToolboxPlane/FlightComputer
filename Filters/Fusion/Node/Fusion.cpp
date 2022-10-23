//
// Created by paul on 14.03.18.
//

#include "Fusion.hpp"

#include "../../../Utilities/time.hpp"
#include "../Lib/DecodePackage.hpp"

namespace filter {
    using namespace si::literals;

    constexpr auto ACC_SIGMA_V = 10.0F * si::acceleration / si::second; // Process noise
    constexpr auto ACC_SIGMA_W = 0.035F * si::acceleration;             // Measurement noise
    constexpr auto CALIB_STAT_THRESH = CalibStatus::NOT_CALIBRATED;

    Fusion::Fusion() :
        lastUpdate{util::time::get()},
        accXFilter{ACC_SIGMA_V, ACC_SIGMA_W},
        accYFilter{ACC_SIGMA_V, ACC_SIGMA_W},
        accZFilter{ACC_SIGMA_V, ACC_SIGMA_W} {
        this->start();
    }

    OutputChannel<FusionResult> &Fusion::getChannelOut() {
        return out;
    }

    InputChannel<GpsMeasurement_t> &Fusion::getGpsIn() {
        return gpsIn;
    }

    InputChannel<FlightControllerPackage> &Fusion::getFlightControllerIn() {
        return flightControllerIn;
    }

    InputChannel<LoraPackage> &Fusion::getRemoteIn() {
        return remoteIn;
    }

    InputChannel<PdbPackage> &Fusion::getPdbIn() {
        return pdbIn;
    }

    InputChannel<TaranisPackage> &Fusion::getTaranisIn() {
        return taranisIn;
    }

    InputChannel<NavPackage> &Fusion::getNavIn() {
        return navIn;
    }

    void Fusion::run() {
        while (!flightControllerIn.isClosed()) {
            while (gpsIn.get(lastGpsMeasurement))
                ;
            while (pdbIn.get(lastPdbPackage))
                ;
            while (taranisIn.get(lastTaranisPackage))
                ;
            while (remoteIn.get(lastRemotePackage))
                ;
            while (navIn.get(lastNavPackage))
                ;

            do {
                flightControllerIn.get(lastFcPackage);
            } while (flightControllerIn.numAvailable() > 0);

            process();
        }

        std::exit(0);
    }

    void Fusion::process() {
        FusionResult res{};

        if (lastPdbPackage.has_value()) {
            res.pdbPackage = lastPdbPackage.value();
        } else {
            std::cout << "No PDB Package received!" << std::endl;
        }

        if (lastTaranisPackage.has_value()) {
            res.taranisPackage = lastTaranisPackage.value();
        } else {
            std::cout << "No Taranis Package received!" << std::endl;
        }

        if (lastRemotePackage.has_value()) {
            res.loraRemote = lastRemotePackage.value();
        } else {
            std::cout << "No Lora-Remote Package received!" << std::endl;
        }

        if (!lastNavPackage.has_value()) {
            std::cerr << "No Nav Package received!" << std::endl;
            // return;
        }

        if (!lastGpsMeasurement.has_value()) {
            std::cerr << "No GPS Package received!" << std::endl;
            return;
        }


        auto flightControllerData = lastFcPackage;
        auto gpsMeasurement = lastGpsMeasurement.value();
        auto navData = NavPackage{}; // fusion::decodePackage<NavPackage>(lastNavPackage.value()); //@TODO hack here

        const auto startTime = util::time::get();
        const auto dtDouble = startTime - lastUpdate;
        const auto dt = static_cast<si::Second<>>(dtDouble);
        lastUpdate = startTime;

        if (!calibration.isCalibrated()) {
            if (gpsMeasurement.fixAquired && flightControllerData.sysCalibStatus >= CALIB_STAT_THRESH &&
                flightControllerData.accCalibStatus >= CALIB_STAT_THRESH &&
                flightControllerData.magCalibStatus >= CALIB_STAT_THRESH &&
                flightControllerData.gyroCalibStatus >= CALIB_STAT_THRESH) {
                calibration.update(startTime, flightControllerData, gpsMeasurement, navData);
            } else {
                std::cout << "Calib not running, reason(s):\n";

                if (!gpsMeasurement.fixAquired) {
                    std::cerr << "\tNo GPS Fix" << std::endl;
                }
                if (flightControllerData.sysCalibStatus < CALIB_STAT_THRESH) {
                    std::cerr << "\tBNO Sys Calib Stat: " << static_cast<int>(flightControllerData.sysCalibStatus)
                              << std::endl;
                }
                if (flightControllerData.magCalibStatus < CALIB_STAT_THRESH) {
                    std::cerr << "\tBNO Mag Calib Stat: " << static_cast<int>(flightControllerData.magCalibStatus)
                              << std::endl;
                }
                if (flightControllerData.accCalibStatus < CALIB_STAT_THRESH) {
                    std::cerr << "\tBNO Acc Calib Stat: " << static_cast<int>(flightControllerData.accCalibStatus)
                              << std::endl;
                }
            }
        } else {
            if (!gpsMeasurement.fixAquired) {
                std::cerr << "No GPS fix, running in pure prediction mode" << std::endl;
            }

            if (flightControllerData.sysCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Sys Calib Stat: " << static_cast<int>(flightControllerData.sysCalibStatus)
                          << std::endl;
            }
            if (flightControllerData.magCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Mag Calib Stat: " << static_cast<int>(flightControllerData.magCalibStatus)
                          << std::endl;
            }
            if (flightControllerData.accCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Acc Calib Stat: " << static_cast<int>(flightControllerData.accCalibStatus)
                          << std::endl;
            }
            if (flightControllerData.gyroCalibStatus < CALIB_STAT_THRESH) {
                std::cerr << "\tBNO Gyro Calib Stat: " << static_cast<int>(flightControllerData.gyroCalibStatus)
                          << std::endl;
            }

            calibration.applyCalib(startTime, flightControllerData, gpsMeasurement, navData);


            auto pfState = particleFilter.update(dt, flightControllerData, gpsMeasurement, navData,
                                                 calibration.getAdditionalBaroUncertainty());

            lastGpsMeasurement->location.lat = NAN;
            lastGpsMeasurement->location.lon = NAN;
            lastGpsMeasurement->location.altitude = NAN * si::meter;
            lastGpsMeasurement->speed = NAN * si::speed;
            lastGpsMeasurement->climb = NAN * si::speed;

            accXFilter.addMeasurement(flightControllerData.accX, dt);
            accYFilter.addMeasurement(flightControllerData.accY, dt);
            accZFilter.addMeasurement(flightControllerData.accZ, dt);

            State state{};
            state.roll = pfState.roll_angle;
            state.rollDeriv = pfState.roll_deriv * si::hertz;
            state.pitch = pfState.pitch_angle;
            state.pitchDeriv = pfState.pitch_deriv * si::hertz;
            state.yaw = pfState.yaw_angle;
            state.yawDeriv = pfState.yaw_deriv * si::hertz;
            state.speed = pfState.speed * si::speed;
            state.position.altitude = pfState.altitude * si::meter;
            state.altitudeAboveGround = pfState.altitude_above_ground * si::meter;
            state.altitudeGround = state.position.altitude - state.altitudeAboveGround;
            state.position.lat = pfState.lat;
            state.position.lon = pfState.lon;
            state.accX = accXFilter.getMeasurementEstimate();
            state.accY = accYFilter.getMeasurementEstimate();
            state.accZ = accZFilter.getMeasurementEstimate();
            state.startTime = calibration.getStartTime();
            state.startLocation = calibration.getStartLocation();

            res.flightControllerPackage = flightControllerData;
            res.navPackage = navData;
            res.state = state;

            std::cout << "dt=" << dt << "\truntime=" << util::time::get() - startTime << std::endl;

            out.put(res);
        }
    }
} // namespace filter
