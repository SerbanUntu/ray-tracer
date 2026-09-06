#include "raytracer/shapes/sphere.h"

Sphere::Sphere(std::unique_ptr<const Material> _mat, const Vec3& _center, const double _radius) :
    Object(std::move(_mat)), center(_center),
    radius(_radius)
{
}

Vec3 Sphere::get_center() const { return center; }

double Sphere::get_radius() const { return radius; }

double Sphere::ray_intersection(const Ray& r) const
{
    const double A = r.direction * r.direction;
    const double B = r.direction * (r.origin - center) * 2;
    const double C = (r.origin - center) * (r.origin - center) - radius * radius;
    const double delta = B * B - 4 * A * C;
    if (delta < 0) return -1.0;
    const double sqrt_delta = sqrt(delta);
    const double t1 = (-B - sqrt_delta) / (2 * A);
    const double t2 = (-B + sqrt_delta) / (2 * A);

    if (t1 >= 0) return t1;
    return t2;
}

Vec3 Sphere::get_normal(const Vec3& point) const
{
    return (point - center).to_normalized();
}
