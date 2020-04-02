/**
 * @file Calibration.hpp
 * @author paul
 * @date 02.04.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_CALIBRATION_HPP
#define FLIGHTCOMPUTER_CALIBRATION_HPP


#include "../../../Utilities/Si/SiExtended.hpp"
#include "../../../Devices/GPS/Type/Gps_t.hpp"
#include "../Type/State_t.hpp"

class Calibration {
    public:
        Calibration();

        void update(si::base::Second<long double> currentTime, const FlightControllerPackage &flightControllerPackage,
                const GpsMeasurement_t &gpsMeasurement, const NavPackage &navPackage);

        [[nodiscard]] auto isCalibrated() const -> bool;

        void applyCalib(si::base::Second<long double> currentTime, FlightControllerPackage &flightControllerPackage,
                        GpsMeasurement_t &gpsMeasurement, NavPackage &navPackage);

        [[nodiscard]] auto getStartLocation() const -> Gps_t;

        [[nodiscard]] auto getStartTime() const -> si::base::Second<long double>;

    private:
        /**
         * Calculates an update for the maximum a-posterior estimate
         * @tparam T type of the random variable and the std deviation
         * @param x_hat a-priori estimate
         * @param p std-deviation of the a-priori
         * @param z measurement
         * @param r std-deviation of the measurement
         */
        template <typename T>
        static void mapUpdate(T &x_hat, T &sqrt_p, T z, T sqrt_r);

        bool calibrationFinished;
        std::size_t numMeas;

        Gps_t startPosition;
        // This is actually in meters as well but it gets treated as actual lat/lon error here.
        double latStdDev, lonStdDev;
        si::base::Meter<> altStdDev;

        si::extended::Acceleration<> accOffset;

        si::base::Meter<> baroOffset;

        si::base::Second<long double> calibTime;

        si::base::Meter<> baroCalibUncertainty{};

        static constexpr auto GPS_STDDEV_THRESH = 3 * si::base::meter;
        static constexpr auto NUM_MEAS_THRESH = 20;
        // Additional uncertainty as uncertainty/distance
        static constexpr auto DISTANCE_ALTITUDE_UNCERTAINTY = (10 * si::base::meter) / (1000 * si::base::meter);
        // Additional uncertainty as uncertainty/time
        static constexpr auto TIME_ALTITUDE_UNCERTAINTY = (10.0F * si::base::meter) / (60 * 60 * si::base::second);
};


#endif //FLIGHTCOMPUTER_CALIBRATION_HPP
