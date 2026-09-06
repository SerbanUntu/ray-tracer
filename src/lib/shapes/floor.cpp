#include "floor.h"

Floor::Floor(std::unique_ptr<const Material> _mat, const double _y) : Object(std::move(_mat)), y(_y)
{
}

double Floor::get_y() const { return y; }

double Floor::ray_intersection(const Ray& r) const
{
    return (y - r.origin.y) / r.direction.y;
}

Vec3 Floor::get_normal(const Vec3& point) const
{
    return {0, 1, 0};
}

std::string Floor::get_type() const { return "Floor"; }
