#pragma once
#include <cmath>

namespace raytracer::common
{
struct Complex
{
    double re;
    double im;

    constexpr Complex(const double _re, const double _im) : re(_re), im(_im)
    {
    }

    static const Complex ZERO;

    [[nodiscard]] constexpr double real_part() const { return re; }
    [[nodiscard]] constexpr double imaginary_part() const { return im; }
    [[nodiscard]] constexpr Complex squared() const { return {re * re - im * im, 2 * re * im}; }
    [[nodiscard]] constexpr Complex conjugate() const { return {re, -im}; }
    [[nodiscard]] double magnitude() const { return std::sqrt(re * re + im * im); }
    [[nodiscard]] constexpr double magnitude_squared() const { return re * re + im * im; }

    constexpr Complex operator-() const { return {-re, -im}; }

    constexpr Complex operator+(Complex const& other) const
    {
        return {re + other.re, im + other.im};
    }

    constexpr Complex operator-(Complex const& other) const
    {
        return {re - other.re, im - other.im};
    }
};

inline constexpr Complex Complex::ZERO = {0, 0};
} // namespace raytracer::common
