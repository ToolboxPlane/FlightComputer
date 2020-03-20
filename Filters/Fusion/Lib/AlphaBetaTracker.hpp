/**
 * @file AlphaBetaTracker.hpp
 * @author paul
 * @date 20.03.20
 * Description here TODO
 */
#ifndef FLIGHTCOMPUTER_ALPHABETATRACKER_HPP
#define FLIGHTCOMPUTER_ALPHABETATRACKER_HPP

/**
 * This class implements an alpha beta filter.
 *
 * @tparam T floating point type to use
 */
template<typename T>
class AlphaBetaTracker {
    public:
        using x_type = T;
        using v_type = decltype(T{}/si::base::second);
        using z_type = x_type;
        using sigma_v_type = v_type;
        using sigma_w_type = z_type;

        AlphaBetaTracker(sigma_v_type sigma_v, sigma_w_type sigma_w,
                x_type x_hat = x_type{0}, v_type v_hat = v_type{0});
        void addMeasurement(x_type z, si::base::Second<> dt);

        auto getStateEstimate() const -> std::pair<x_type, v_type>;
        auto getMeasurementEstimate() const -> z_type;

    private:
        sigma_v_type sigma_v;
        sigma_w_type sigma_w;
        x_type x_hat;
        v_type v_hat;
};

template<typename T>
AlphaBetaTracker<T>::AlphaBetaTracker(sigma_v_type sigma_v, sigma_w_type sigma_w, x_type x_hat, v_type v_hat)
    : sigma_v{sigma_v}, sigma_w{sigma_w}, x_hat{x_hat}, v_hat{v_hat} {}

template<typename T>
void AlphaBetaTracker<T>::addMeasurement(z_type z, si::base::Second<> dt) {
    // Calculate Kalman Gain
    const auto lambda = static_cast<typename T::type>(sigma_v * dt * dt / sigma_w);
    const auto lambda2 = lambda * lambda;
    const auto alpha = -0.125 * (lambda2 + 8 * lambda - (lambda + 4) * (std::sqrt(lambda2 + 8 * lambda)));
    const auto beta = 0.25 * (lambda2 + 4 * lambda - lambda * std::sqrt(lambda2 + 8 * lambda));

    // Prediction
    x_hat = x_hat + dt * v_hat;
    const auto z_hat = x_hat;

    // Innovation
    const auto k1 = alpha;
    const auto k2 = beta / dt;
    const auto gamma = z - z_hat;
    x_hat = x_hat + k1 * gamma;
    v_hat = v_hat + k2 * gamma;
}

template<typename T>
auto AlphaBetaTracker<T>::getStateEstimate() const -> std::pair<x_type, v_type> {
    return {x_hat, v_hat};
}

template<typename T>
auto AlphaBetaTracker<T>::getMeasurementEstimate() const -> z_type {
    return x_hat;
}


#endif //FLIGHTCOMPUTER_ALPHABETATRACKER_HPP
