#pragma once
#include "util/vec3.h"
#include "camera.h"
#include "object.h"

class Sphere : public Object
{
    Vec3 center;
    double radius;

public:
    Sphere(std::unique_ptr<const Material> _mat, const Vec3& _center, double _radius);
    [[nodiscard]] Vec3 get_center() const;
    [[nodiscard]] double get_radius() const;
    [[nodiscard]] double ray_intersection(const Ray& r) const override;
    [[nodiscard]] Vec3 get_normal(const Vec3& point) const override;
    [[nodiscard]] std::string get_type() const override;
};
