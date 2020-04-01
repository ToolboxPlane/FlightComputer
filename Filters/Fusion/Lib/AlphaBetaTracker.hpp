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
        using f_type = typename T::type;
        using x_type = T;
        using v_type = decltype(T{}/si::base::Second<f_type>{});
        using z_type = x_type;
        using sigma_v_type = v_type;
        using sigma_w_type = z_type;

        AlphaBetaTracker(sigma_v_type sigma_v, sigma_w_type sigma_w,
                x_type x_hat = x_type{0}, v_type v_hat = v_type{0});
        void addMeasurement(z_type z, si::base::Second<> dt);

        auto getStateEstimate() const -> std::pair<x_type, v_type>;
        auto getMeasurementEstimate() const -> z_type;

        auto getNIS() const -> f_type;
        constexpr auto getNIS95Bound() const -> f_type;

    private:
        sigma_v_type sigma_v;
        sigma_w_type sigma_w;
        x_type x_hat;
        v_type v_hat;
        f_type nis;
};

template<typename T>
AlphaBetaTracker<T>::AlphaBetaTracker(sigma_v_type sigma_v, sigma_w_type sigma_w, x_type x_hat, v_type v_hat)
    : sigma_v{sigma_v}, sigma_w{sigma_w}, x_hat{x_hat}, v_hat{v_hat} {}

template<typename T>
void AlphaBetaTracker<T>::addMeasurement(z_type z, si::base::Second<> dt) {
    // Calculate Kalman Gain
    const auto lambda = static_cast<f_type >(sigma_v * dt * dt / sigma_w);
    const auto lambda2 = lambda * lambda;
    const auto alpha = -0.125F * (lambda2 + 8 * lambda - (lambda + 4) * (std::sqrt(lambda2 + 8 * lambda)));
    const auto beta = 0.25F * (lambda2 + 4 * lambda - lambda * std::sqrt(lambda2 + 8 * lambda));

    // Prediction
    x_hat = x_hat + dt * v_hat;
    const auto z_hat = x_hat;

    // Innovation
    const auto k1 = alpha;
    const auto k2 = beta / dt;
    const auto gamma = z - z_hat;
    x_hat = x_hat + k1 * gamma;
    v_hat = v_hat + k2 * gamma;

    // NIS Calculation
    /*
     *      \alpha = m_{11} / (m_{11} + \sigma_w^2)
     * <=>  m_{11} = \alpha m_{11} + \sigma_w^2 * \alpha
     * <=>  m_{11} * (1 - \alpha) = \sigma_w^2 * \alpha
     * <=>  m_{11} = \sigma_w^2 / (1 - \alpha)
     *      S = s_{11} = m_{11} + \sigma_w^2
     *        = \sigma_w^2 * (1/(1-\alpha) + 1)
     *        = \sigma_w^2 * (2-\alpha)/(1-\alpha)
     *      NIS = \sqrt{\T{\hat{z}} * S^{-1} * \hat{z}}
     *          = \abs{hat{z}} / \sqrt{s_{11}}
     */
    const auto s = sigma_w * sigma_w * (2-alpha)/(1-alpha);
    nis = std::abs(static_cast<f_type>(z_hat)) / std::sqrt(static_cast<f_type>(s));
}

template<typename T>
auto AlphaBetaTracker<T>::getStateEstimate() const -> std::pair<x_type, v_type> {
    return {x_hat, v_hat};
}

template<typename T>
auto AlphaBetaTracker<T>::getMeasurementEstimate() const -> z_type {
    return x_hat;
}

template<typename T>
auto AlphaBetaTracker<T>::getNIS() const -> f_type {
    return nis;
}

template<typename T>
constexpr auto AlphaBetaTracker<T>::getNIS95Bound() const -> f_type {
    return 3.84F; // Chi^2 0.95 bound
}


#endif //FLIGHTCOMPUTER_ALPHABETATRACKER_HPP
