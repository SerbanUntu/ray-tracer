#include "raytracer/shapes/floor.h"

namespace raytracer::raytracer
{
Floor::Floor(std::unique_ptr<const Material> _mat, const double _y) : Object(std::move(_mat)), y(_y)
{
}

double Floor::get_y() const { return y; }

double Floor::ray_intersection(const Ray& r) const
{
    if (r.direction.y == 0) return -1.0;
    return (y - r.origin.y) / r.direction.y;
}

common::Vec3 Floor::get_normal(const common::Vec3& point) const
{
    return {0, 1, 0};
}
} // namespace raytracer::raytracer
