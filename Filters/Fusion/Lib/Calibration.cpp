/**
 * @file Calibration.cpp
 * @author paul
 * @date 02.04.20
 * Description here TODO
 */
#include "Calibration.hpp"

#include <SiStl.hpp>

Calibration::Calibration() : calibrationFinished{false}, numMeas{0}, startPosition{0, 0, 0 * si::meter},
                             lastPosition{0, 0},
                             latStdDev{NAN}, lonStdDev{NAN}, altStdDev{NAN}, accOffset{0}, baroOffset{0},
                             calibTime{0} {}

void
Calibration::update(si::Second<long double> currentTime, const FlightControllerPackage &flightControllerPackage,
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
        //if (lastGpsTimestamp < gpsMeasurement.timestamp) {
        mapUpdate(startPosition.lat, latStdDev,
                  gpsMeasurement.location.lat, static_cast<double>(gpsMeasurement.epLat / 2));
        mapUpdate(startPosition.lon, lonStdDev,
                  gpsMeasurement.location.lon, static_cast<double>(gpsMeasurement.epLon / 2));
        mapUpdate(startPosition.altitude, altStdDev, gpsMeasurement.location.altitude, gpsMeasurement.epVert / 2);
        //}
    }
    lastGpsTimestamp = gpsMeasurement.timestamp;

    auto accSqr = flightControllerPackage.accX * flightControllerPackage.accX
                  + flightControllerPackage.accY * flightControllerPackage.accY
                  + flightControllerPackage.accZ * flightControllerPackage.accZ;
    auto accAbs = std::sqrt(accSqr);

    accOffset += accAbs;

    baroOffset += navPackage.baroAltitude;

    numMeas += 1;
    if (si::Meter<>{static_cast<float>(latStdDev)} < GPS_STDDEV_THRESH &&
        si::Meter<>{static_cast<float>(lonStdDev)} < GPS_STDDEV_THRESH
        && altStdDev < GPS_STDDEV_THRESH && numMeas > NUM_MEAS_THRESH) {
        accOffset = accOffset / numMeas;
        baroOffset = baroOffset / numMeas - gpsMeasurement.location.altitude;

        calibTime = currentTime;
        calibrationFinished = true;

        std::cout << "Calibration finished after" << numMeas << " measurements:\n"
                  << "\t\\baroOffset=" << baroOffset << "\n"
                  << "\t\\accOffset=" << accOffset << "\n"
                  << "\t\\sigma_lat=" << si::Meter<>{static_cast<float>(latStdDev)} << "\n"
                  << "\t\\sigma_lon=" << si::Meter<>{static_cast<float>(lonStdDev)} << "\n"
                  << "\t\\sigma_alt=" << si::Meter<>{static_cast<float>(altStdDev)} << std::endl;
    } else {
        std::cout << "Calibration not finished after " << numMeas << " measurements:\n"
                  << "\t\\sigma_lat=" << si::Meter<>{static_cast<float>(latStdDev)} << "\n"
                  << "\t\\sigma_lon=" << si::Meter<>{static_cast<float>(lonStdDev)} << "\n"
                  << "\t\\sigma_alt=" << si::Meter<>{static_cast<float>(altStdDev)} << std::endl;
    }
}

void
Calibration::applyCalib(si::Second<long double> currentTime, FlightControllerPackage &flightControllerPackage,
                        GpsMeasurement_t &gpsMeasurement, NavPackage &navPackage) {
    // Acceleration
    auto xWeight = static_cast<float>(std::sin(flightControllerPackage.pitch / 180 * M_PI));
    auto yWeight = static_cast<float>(std::sin(-flightControllerPackage.roll / 180 * M_PI) *
                                      std::cos(flightControllerPackage.pitch / 180 * M_PI));
    auto zWeight = std::sqrt(1 - xWeight * xWeight - yWeight * yWeight);

    flightControllerPackage.accX -= accOffset * xWeight;
    flightControllerPackage.accY -= accOffset * yWeight;
    flightControllerPackage.accZ -= accOffset * zWeight;

    // Barometer
    navPackage.baroAltitude -= baroOffset;

    if (!std::isnan(gpsMeasurement.location.lat) && !std::isnan(gpsMeasurement.location.lon) &&
        !std::isnan(gpsMeasurement.location.altitude)) {
        lastPosition = gpsMeasurement.location;
    }

    auto distToStart = lastPosition.distanceTo(startPosition);
    auto timeToStart = static_cast<si::Second<>>(currentTime - calibTime);

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
    sqrt_p = std::sqrt(p);
}

auto Calibration::getStartLocation() const -> Gps_t {
    return startPosition;
}

auto Calibration::getStartTime() const -> si::Second<long double> {
    return calibTime;
}

auto Calibration::getAdditionalBaroUncertainty() const -> si::Meter<> {
    return baroCalibUncertainty;
}
