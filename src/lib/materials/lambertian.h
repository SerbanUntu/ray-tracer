#pragma once
#include <string>
#include "material.h"
#include "util/vec3.h"

class Lambertian : public Material
{
    Vec3 albedo;

public:
    Lambertian();
    explicit Lambertian(const Vec3& _albedo);
    [[nodiscard]] Ray get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] Vec3 get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] Vec3 get_albedo() const;
    [[nodiscard]] std::string get_type() const override;
};
