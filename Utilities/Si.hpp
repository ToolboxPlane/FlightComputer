/**
 * @file Si.hpp
 * @author paul
 * @date 20.03.19
 * @brief Si @TODO
 */

#ifndef FLIGHTCOMPUTER_SI_HPP
#define FLIGHTCOMPUTER_SI_HPP

#include <list>

namespace si {
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T = long double>
    class Si {
        using ThisT = Si<m,kg,s,A,K,MOL,CD,T>;
    public:
        explicit Si(T val) noexcept;
        explicit operator T() const;

        // Add (only same units)
        auto operator+(ThisT rhs) const -> ThisT;
        void operator+=(ThisT rhs);

        // Subtract (only same units)
        auto operator-(ThisT rhs) const -> ThisT;
        void operator-=(ThisT rhs);

        // Multiply with Unitless
        auto operator*(T rhs) const -> Si<m,kg,s,A,K,MOL,CD,T>;
        template<int m_, int kg_, int s_, int A_, int K_, int MOL_, int CD_, typename T_>
        friend auto operator*(T_ lhs, Si<m_,kg_,s_,A_,K_,MOL_,CD_,T_> rhs) -> Si<m_,kg_,s_,A_,K_,MOL_,CD_,T_>;

        // Divide by Unitless
        auto operator/(T rhs) const -> Si<m,kg,s,A,K,MOL,CD,T>;
        template<int m_, int kg_, int s_, int A_, int K_, int MOL_, int CD_, typename T_>
        friend auto operator/(T_ lhs, Si<m_,kg_,s_,A_,K_,MOL_,CD_,T_> rhs) -> Si<m_,kg_,s_,A_,K_,MOL_,CD_,T_>;

        // Multiply with Unit
        template<int m_, int kg_, int s_, int A_, int K_, int MOL_, int CD_>
        auto operator*(Si<m_,kg_,s_,A_,K_,MOL_,CD_,T> rhs) const -> Si<m+m_,kg+kg_,s+s_,A+A_,K+K_,MOL+MOL_,CD+CD_,T>;
    private:
        T val;
    };

    // CTor, Conversion
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    Si<m, kg, s, A, K, MOL, CD, T>::Si(T val) noexcept : val{val} {}

    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    Si<m, kg, s, A, K, MOL, CD, T>::operator T() const {
        return val;
    }

    // Add
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    auto Si<m, kg, s, A, K, MOL, CD, T>::operator+(Si::ThisT rhs) const -> Si::ThisT {
        return ThisT{this->val + rhs.val};
    }

    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    void Si<m, kg, s, A, K, MOL, CD, T>::operator+=(Si::ThisT rhs) {
        this->val += rhs.val;
    }

    // Subtract
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    auto Si<m, kg, s, A, K, MOL, CD, T>::operator-(Si::ThisT rhs) const -> Si::ThisT {
        return ThisT{this->val - rhs.val};
    }

    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    void Si<m, kg, s, A, K, MOL, CD, T>::operator-=(Si::ThisT rhs) {
        this->val -= rhs.val;
    }

    // Multiply by scalar
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    auto Si<m, kg, s, A, K, MOL, CD, T>::operator*(T rhs) const -> Si<m, kg, s, A, K, MOL, CD, T> {
        return ThisT{this->val * rhs};
    }

    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    auto operator*(T lhs, Si<m, kg, s, A, K, MOL, CD, T> rhs) -> Si<m, kg, s, A, K, MOL, CD, T> {
        return Si<m,kg,s,A,K,MOL,CD>{lhs * rhs.val};
    }

    // Divide by scalar
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    auto Si<m, kg, s, A, K, MOL, CD, T>::operator/(T rhs) const -> Si<m, kg, s, A, K, MOL, CD, T> {
        return ThisT{this->val / rhs};
    }

    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    auto operator/(T lhs, Si<m, kg, s, A, K, MOL, CD, T> rhs) -> Si<m, kg, s, A, K, MOL, CD, T> {
        return {lhs / rhs.val};
    }

    // Multiply with different type
    template<int m, int kg, int s, int A, int K, int MOL, int CD, typename T>
    template<int m_, int kg_, int s_, int A_, int K_, int MOL_, int CD_>
    auto Si<m, kg, s, A, K, MOL, CD, T>::operator*(Si<m_, kg_, s_, A_, K_, MOL_, CD_, T> rhs) const -> Si<
            m + m_, kg + kg_, s + s_, A + A_, K + K_, MOL + MOL_, CD + CD_, T> {
        return {this->val * rhs.val};
    }


    namespace literals {
        // Prefix
        constexpr long double operator "" _Y(long double t) { return t * 1E24; }
        constexpr long double operator "" _Z(long double t) { return t * 1E21; }
        constexpr long double operator "" _E(long double t) { return t * 1E18; }
        constexpr long double operator "" _P(long double t) { return t * 1E15; }
        constexpr long double operator "" _T(long double t) { return t * 1E12; }
        constexpr long double operator "" _G(long double t) { return t * 1E9; }
        constexpr long double operator "" _M(long double t) { return t * 1E6; }
        constexpr long double operator "" _k(long double t) { return t * 1E3; }
        constexpr long double operator "" _h(long double t) { return t * 1E2; }
        constexpr long double operator "" _da(long double t) { return t * 1E1; }
        constexpr long double operator "" _d(long double t) { return t * 1E-1; }
        constexpr long double operator "" _c(long double t) { return t * 1E-2; }
        constexpr long double operator "" _m(long double t) { return t * 1E-3; }
        constexpr long double operator "" _mu(long double t) { return t * 1E-6; }
        constexpr long double operator "" _n(long double t) { return t * 1E-9; }
        constexpr long double operator "" _p(long double t) { return t * 1E-12; }
        constexpr long double operator "" _f(long double t) { return t * 1E-15; }
        constexpr long double operator "" _a(long double t) { return t * 1E-18; }
        constexpr long double operator "" _z(long double t) { return t * 1E-21; }
        constexpr long double operator "" _y(long double t) { return t * 1E-24; }

        // Base units
        const Si<1,0,0,0,0,0,0> meter{1};
        const Si<0,1,0,0,0,0,0> kilogram{1};
        const Si<0,0,1,0,0,0,0> second{1};
        const Si<0,0,0,1,0,0,0> ampere{1};
        const Si<0,0,0,0,1,0,0> kelvin{1};
        const Si<0,0,0,0,0,1,0> mole{1};
        const Si<0,0,0,0,0,0,1> candela{1};
    }
}

#endif //FLIGHTCOMPUTER_SI_HPP
