#include "lambertian.h"
#include "util/random_utils.h"

Lambertian::Lambertian() : albedo(Vec3::ZERO)
{
}

Lambertian::Lambertian(const Vec3& _albedo) : albedo(_albedo)
{
}

Ray Lambertian::get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    return Ray(intersection, (normal + random_unit()).to_normalized());
}

Vec3 Lambertian::get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    return albedo;
}

Vec3 Lambertian::get_albedo() const
{
    return albedo;
}
