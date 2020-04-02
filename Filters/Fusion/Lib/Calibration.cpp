/**
 * @file Calibration.cpp
 * @author paul
 * @date 02.04.20
 * Description here TODO
 */
#include "Calibration.hpp"

Calibration::Calibration() : calibrationFinished{false}, numMeas{0}, startPosition{0, 0, 0},
                             latStdDev{0}, lonStdDev{0}, altStdDev{0}, accOffset{0}, baroOffset{0}, calibTime{0} {}

void
Calibration::update(si::base::Second<long double> currentTime, const FlightControllerPackage &flightControllerPackage,
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

    numMeas += 1;
    if (si::base::Meter<>{static_cast<float>(latStdDev)} < GPS_STDDEV_THRESH &&
        si::base::Meter<>{static_cast<float>(lonStdDev)} < GPS_STDDEV_THRESH
        && altStdDev < GPS_STDDEV_THRESH && numMeas > NUM_MEAS_THRESH) {
        accOffset = accOffset / numMeas;
        baroOffset = baroOffset / numMeas - gpsMeasurement.location.altitude;

        calibTime = currentTime;
        calibrationFinished = true;
    } else {
        std::cout << "Calibration not finished after " << numMeas << " measurements:\n"
                  << "\t\\sigma_lat=" << si::base::Meter<>{static_cast<float>(latStdDev)} << "\n"
                  << "\t\\sigma_lon=" << si::base::Meter<>{static_cast<float>(lonStdDev)} << "\n"
                  << "\t\\sigma_alt=" << si::base::Meter<>{static_cast<float>(altStdDev)} << std::endl;
    }
}

void
Calibration::applyCalib(si::base::Second<long double> currentTime, FlightControllerPackage &flightControllerPackage,
                        GpsMeasurement_t &gpsMeasurement, NavPackage &navPackage) {
    // Acceleration
    auto xWeight = static_cast<float>(std::sin(-flightControllerPackage.pitch / 180 * M_PI));
    auto yWeight = static_cast<float>(std::sin(flightControllerPackage.roll / 180 * M_PI));
    auto zWeight = static_cast<float>(std::cos(-flightControllerPackage.pitch / 180 * M_PI)
                   * std::cos(flightControllerPackage.roll / 180 * M_PI));
    flightControllerPackage.accX -= accOffset * xWeight;
    flightControllerPackage.accY -= accOffset * yWeight;
    flightControllerPackage.accZ -= accOffset * zWeight;

    // Barometer
    navPackage.baroAltitude -= baroOffset;
    auto distToStart = gpsMeasurement.location.distanceTo(startPosition);
    auto timeToStart = static_cast<si::base::Second<>>(currentTime - calibTime);

    baroCalibUncertainty = distToStart * DISTANCE_ALTITUDE_UNCERTAINTY + timeToStart * TIME_ALTITUDE_UNCERTAINTY;
}


auto Calibration::isCalibrated() const -> bool {
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

auto Calibration::getStartLocation() const -> Gps_t {
    return startPosition;
}

auto Calibration::getStartTime() const -> si::base::Second<long double> {
    return calibTime;
}

