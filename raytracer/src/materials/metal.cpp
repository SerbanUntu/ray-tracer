#include "raytracer/materials/metal.h"
#include "common/util/random_utils.h"

namespace raytracer::raytracer
{
Metal::Metal(const common::Vec3& _albedo, const double _fuzz) : albedo(_albedo), fuzz(_fuzz)
{
}

Ray Metal::get_scattered(const Ray& ray_in, const common::Vec3& intersection, const common::Vec3& normal) const
{
    return Ray(intersection, (reflect(ray_in.direction, normal) + common::random_unit() * fuzz).to_normalized());
}

common::Vec3 Metal::get_color(const Ray& ray_in, const common::Vec3& intersection, const common::Vec3& normal) const
{
    return albedo;
}

common::Vec3 Metal::get_albedo() const
{
    return albedo;
}

double Metal::get_fuzz() const
{
    return fuzz;
}
} // namespace raytracer::raytracer
