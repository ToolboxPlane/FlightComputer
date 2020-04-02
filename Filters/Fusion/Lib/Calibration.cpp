/**
 * @file Calibration.cpp
 * @author paul
 * @date 02.04.20
 * Description here TODO
 */
#include "Calibration.hpp"

Calibration::Calibration() : calibrationFinished{false}, numMeas{0}, startPosition{0, 0, 0},
                             latStdDev{0}, lonStdDev{0}, altStdDev{0}, accOffset{0}, baroOffset{0}, calibTime{0} {}

void Calibration::update(si::base::Second<long double> currentTime, const FlightControllerPackage &flightControllerPackage,
                         const GpsMeasurement_t &gpsMeasurement, const NavPackage &navPackage) {
    if (calibrationFinished) {
        return;
    }

    if (numMeas <= 0) {
        startPosition = gpsMeasurement.location;
        latStdDev = static_cast<double>(gpsMeasurement.epLat / 2);
        lonStdDev = static_cast<double>(gpsMeasurement.epLon / 2);
        altStdDev = gpsMeasurement.epVert / 2;
    } else {
        mapUpdate(startPosition.lat, latStdDev,
                  gpsMeasurement.location.lat, static_cast<double>(gpsMeasurement.epLat / 2));
        mapUpdate(startPosition.lon, lonStdDev,
                  gpsMeasurement.location.lon, static_cast<double>(gpsMeasurement.epLon / 2));
        mapUpdate(startPosition.altitude, altStdDev, gpsMeasurement.location.altitude, gpsMeasurement.epVert / 2);
    }

    auto accSqr = flightControllerPackage.accX * flightControllerPackage.accX
                  + flightControllerPackage.accY * flightControllerPackage.accY
                  + flightControllerPackage.accZ * flightControllerPackage.accZ;
    auto accAbs = std::sqrt(static_cast<si::default_type>(accSqr)) * si::extended::acceleration;

    accOffset += accAbs;

    baroOffset += navPackage.baroAltitude;


    if (si::base::Meter<>{static_cast<float>(latStdDev)} < GPS_STDDEV_THRESH &&
        si::base::Meter<>{static_cast<float>(lonStdDev)} < GPS_STDDEV_THRESH
        && altStdDev < GPS_STDDEV_THRESH && numMeas > NUM_MEAS_THRESH) {
        accOffset = accOffset / numMeas;
        baroOffset = startPosition.altitude - baroOffset / numMeas;

        calibTime = currentTime;
        calibrationFinished = true;
    }
}

auto Calibration::isCalibrated() -> bool {
    return calibrationFinished;
}

template<typename T>
void Calibration::mapUpdate(T &x_hat, T &sqrt_p, T z, T sqrt_r) {
    auto p = sqrt_p * sqrt_p; // Variance of the estimate
    auto r = sqrt_r * sqrt_r; // Variance of the measurement
    auto k = static_cast<si::default_type>(p / (p + r)); // Kalman gain
    auto gamma = z - x_hat; // Measurement error
    x_hat = x_hat + k * gamma; // State update
    p = (1 - k) * p; // Variance update
    sqrt_p = T{std::sqrt(static_cast<si::default_type>(p))};
}
