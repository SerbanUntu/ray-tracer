#pragma once
#include <memory>
#include "common/util/vec3.h"
#include "../materials/material.h"

namespace raytracer::raytracer
{
enum class ObjectType : uint8_t
{
    FLOOR,
    SPHERE
};

class Object
{
    std::unique_ptr<const Material> mat;

public:
    virtual ~Object() = default;

    explicit Object(std::unique_ptr<const Material> _mat) : mat(std::move(_mat))
    {
    }

    [[nodiscard]] const Material* get_material() const { return mat.get(); }
    [[nodiscard]] virtual double ray_intersection(const Ray& r) const = 0;
    [[nodiscard]] virtual common::Vec3 get_normal(const common::Vec3& point) const = 0;
    [[nodiscard]] virtual ObjectType get_type() const = 0;
};
} // namespace raytracer::raytracer
