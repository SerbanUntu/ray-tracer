#pragma once
#include "material.h"
#include "common/util/vec3.h"

namespace raytracer::raytracer
{
class Lambertian : public Material
{
    common::Vec3 albedo;

public:
    Lambertian();
    explicit Lambertian(const common::Vec3& _albedo);
    [[nodiscard]] Ray get_scattered(const Ray& ray_in, const common::Vec3& intersection,
                                    const common::Vec3& normal) const override;
    [[nodiscard]] common::Vec3 get_color(const Ray& ray_in, const common::Vec3& intersection,
                                         const common::Vec3& normal) const override;
    [[nodiscard]] common::Vec3 get_albedo() const;
    [[nodiscard]] MaterialType get_type() const override { return MaterialType::LAMBERTIAN; }
};
} // namespace raytracer::raytracer
