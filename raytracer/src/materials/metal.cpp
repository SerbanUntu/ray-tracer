#include "raytracer/materials/metal.h"
#include "common/util/random_utils.h"

Metal::Metal(const Vec3& _albedo, const double _fuzz) : albedo(_albedo), fuzz(_fuzz)
{
}

Ray Metal::get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    return Ray(intersection, (reflect(ray_in.direction, normal) + random_unit() * fuzz).to_normalized());
}

Vec3 Metal::get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    return albedo;
}

Vec3 Metal::get_albedo() const
{
    return albedo;
}

double Metal::get_fuzz() const
{
    return fuzz;
}
