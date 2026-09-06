#pragma once
#include "object.h"

class Floor : public Object
{
    double y;

public:
    Floor(std::unique_ptr<const Material> _mat, double _y);
    [[nodiscard]] double get_y() const;
    [[nodiscard]] double ray_intersection(const Ray& r) const override;
    [[nodiscard]] Vec3 get_normal(const Vec3& point) const override;
    [[nodiscard]] std::string get_type() const override;
};
