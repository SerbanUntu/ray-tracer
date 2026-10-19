#pragma once
#include <cmath>

namespace raytracer::common
{

/**
 * An imaginary number, with a real and an imaginary part. Both are doubles.
 */
struct Complex
{
    double re;
    double im;

    constexpr Complex(const double _re, const double _im) : re(_re), im(_im)
    {
    }

    /**
     * A complex number where both the real part and the imaginary part are set to zero.
     */
    static const Complex ZERO;

    /**
     * Getter for the real part of the complex number.
     *
     * @return The real part of the complex number.
     */
    [[nodiscard]] constexpr double real_part() const
    {
        return re;
    }

    /**
     * Getter for the imaginary part of the complex number.
     *
     * @return The imaginary part of the complex number.
     */
    [[nodiscard]] constexpr double imaginary_part() const
    {
        return im;
    }

    /**
     * Returns the result of multiplying the complex number with itself.
     *
     * @return The result of multiplying the complex number with itself.
     */
    [[nodiscard]] constexpr Complex squared() const
    {
        return {re * re - im * im, 2 * re * im};
    }

    /**
     * Returns the complex conjugate of the complex number, with has the same real part and minus the imaginary part.
     *
     * @return The complex conjugate of the complex number, with has the same real part and minus the imaginary part.
     */
    [[nodiscard]] constexpr Complex conjugate() const
    {
        return {re, -im};
    }

    /**
     * Returns the magnitude of the complex number, given by re^2 + im^2.
     *
     * @return The magnitude of the complex number.
     */
    [[nodiscard]] double magnitude() const
    {
        return std::sqrt(re * re + im * im);
    }

    /**
     * Removes an expensive std::sqrt operation.
     *
     * @return The squared magnitude of the complex number.
     */
    [[nodiscard]] constexpr double magnitude_squared() const
    {
        return re * re + im * im;
    }

    constexpr Complex operator-() const
    {
        return {-re, -im};
    }

    constexpr Complex operator+(Complex const &other) const
    {
        return {re + other.re, im + other.im};
    }

    constexpr Complex operator-(Complex const &other) const
    {
        return {re - other.re, im - other.im};
    }
};

inline constexpr Complex Complex::ZERO = {0, 0};
} // namespace raytracer::common
