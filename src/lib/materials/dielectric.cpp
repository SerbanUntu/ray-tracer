#include "dielectric.h"
#include <cmath>

Dielectric::Dielectric(const double eta) : refractive_index(eta)
{
}

Ray Dielectric::get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    const double cosine = ray_in.direction.to_normalized() * normal.to_normalized();

    constexpr double eta_in = AIR_REFRACTIVE_INDEX;
    const double eta_out = refractive_index;
    double ratio = eta_in / eta_out;

    // Ray comes from outside the object
    if (cosine > 0)
    {
        ratio = 1. / ratio;
    }

    const Vec3 uv = ray_in.direction.to_normalized();
    const Vec3 n = normal.to_normalized();

    const double cos = std::min(-uv * n, 1.);
    const double sin = std::sqrt(1. - std::pow(cos, 2));

    // Cannot refract
    if (sin * ratio > 1.0)
    {
        return Ray(intersection, reflect(ray_in.direction, normal));
    }

    return Ray(intersection, refract(ray_in.direction, normal, ratio));
}

Vec3 Dielectric::get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    return {1, 1, 1};
}

double Dielectric::get_refractive_index() const
{
    return refractive_index;
}
