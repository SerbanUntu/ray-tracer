#pragma once
#include <iostream>

struct Vec3
{
    double x;
    double y;
    double z;

    constexpr Vec3(const double _x, const double _y, const double _z) : x(_x), y(_y), z(_z)
    {
    }

    static const Vec3 ZERO;

    void normalize()
    {
        const double denominator = length();
        x /= denominator;
        y /= denominator;
        z /= denominator;
    }

    [[nodiscard]] Vec3 to_normalized() const
    {
        const double denominator = length();
        return {x / denominator, y / denominator, z / denominator};
    }

    [[nodiscard]] double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] constexpr Vec3 cross(Vec3 const& other) const
    {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    constexpr bool operator==(Vec3 const& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr Vec3 operator+(Vec3 const& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    constexpr void operator+=(Vec3 const& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    constexpr Vec3 operator-(Vec3 const& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    constexpr Vec3 operator-() const
    {
        return {-x, -y, -z};
    }

    constexpr double operator*(Vec3 const& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr Vec3 operator*(const double d) const
    {
        return {x * d, y * d, z * d};
    }

    constexpr Vec3 operator/(const double d) const
    {
        return {x / d, y / d, z / d};
    }
};

inline std::ostream& operator<<(std::ostream& os, Vec3 const& v)
{
    return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

inline constexpr Vec3 Vec3::ZERO = {0, 0, 0};
