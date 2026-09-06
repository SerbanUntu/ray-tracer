#pragma once
#include "common/util/vec3.h"
#include "../camera.h"
#include "object.h"

namespace raytracer::raytracer
{
class Sphere : public Object
{
    common::Vec3 center;
    double radius;

public:
    Sphere(std::unique_ptr<const Material> _mat, const common::Vec3& _center, double _radius);
    [[nodiscard]] common::Vec3 get_center() const;
    [[nodiscard]] double get_radius() const;
    [[nodiscard]] double ray_intersection(const Ray& r) const override;
    [[nodiscard]] common::Vec3 get_normal(const common::Vec3& point) const override;
    [[nodiscard]] ObjectType get_type() const override { return ObjectType::SPHERE; }
};
} // namespace raytracer::raytracer
