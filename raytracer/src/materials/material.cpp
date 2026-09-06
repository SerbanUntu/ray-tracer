#include "raytracer/materials/material.h"
#include <cmath>

Vec3 Material::reflect(const Vec3& dir_in, const Vec3& normal)
{
    const Vec3 v = dir_in.to_normalized();
    const Vec3 n = normal.to_normalized();

    return (v - n * 2 * (v * n)).to_normalized();
}

Vec3 Material::refract(const Vec3& dir_in, const Vec3& normal, const double refractive_index)
{
    const Vec3 v = dir_in.to_normalized();
    const Vec3 n = normal.to_normalized();

    const double cosine = std::min(-v * n, 1.);

    const Vec3 r_perp = (v + n * cosine) * refractive_index;

    const Vec3 r_para = -n * std::sqrt(std::abs(1 - std::pow(r_perp.length(), 2)));
    return (r_perp + r_para).to_normalized();
}
