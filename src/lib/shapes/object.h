#pragma once
#include <string>
#include <memory>
#include "util/vec3.h"
#include "materials/material.h"

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
    [[nodiscard]] virtual Vec3 get_normal(const Vec3& point) const = 0;
    [[nodiscard]] virtual std::string get_type() const = 0;
};
