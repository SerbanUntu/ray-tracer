#include "raytracer/materials/lambertian.h"
#include "common/util/random_utils.h"

namespace raytracer::raytracer
{
Lambertian::Lambertian() : albedo(common::Vec3::ZERO)
{
}

Lambertian::Lambertian(const common::Vec3& _albedo) : albedo(_albedo)
{
}

Ray Lambertian::get_scattered(const Ray& ray_in, const common::Vec3& intersection, const common::Vec3& normal) const
{
    return Ray(intersection, (normal + common::random_unit()).to_normalized());
}

common::Vec3 Lambertian::get_color(const Ray& ray_in, const common::Vec3& intersection, const common::Vec3& normal) const
{
    return albedo;
}

common::Vec3 Lambertian::get_albedo() const
{
    return albedo;
}
} // namespace raytracer::raytracer
