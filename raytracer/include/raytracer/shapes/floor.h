#pragma once
#include "object.h"

namespace raytracer::raytracer
{
class Floor : public Object
{
    double y;

public:
    Floor(std::unique_ptr<const Material> _mat, double _y);
    [[nodiscard]] double get_y() const;
    [[nodiscard]] double ray_intersection(const Ray& r) const override;
    [[nodiscard]] common::Vec3 get_normal(const common::Vec3& point) const override;
    [[nodiscard]] ObjectType get_type() const override { return ObjectType::FLOOR; }
};
} // namespace raytracer::raytracer
