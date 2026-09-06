#include "raytracer/materials/material.h"
#include <cmath>

namespace raytracer::raytracer
{
common::Vec3 Material::reflect(const common::Vec3& dir_in, const common::Vec3& normal)
{
    const common::Vec3 v = dir_in.to_normalized();
    const common::Vec3 n = normal.to_normalized();

    return (v - n * 2 * (v * n)).to_normalized();
}

common::Vec3 Material::refract(const common::Vec3& dir_in, const common::Vec3& normal, const double refractive_index)
{
    const common::Vec3 v = dir_in.to_normalized();
    const common::Vec3 n = normal.to_normalized();

    const double cosine = std::min(-v * n, 1.);

    const common::Vec3 r_perp = (v + n * cosine) * refractive_index;

    const common::Vec3 r_para = -n * std::sqrt(std::abs(1 - std::pow(r_perp.length(), 2)));
    return (r_perp + r_para).to_normalized();
}
} // namespace raytracer::raytracer
