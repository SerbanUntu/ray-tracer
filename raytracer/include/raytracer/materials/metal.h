#pragma once
#include "material.h"

namespace raytracer::raytracer
{
class Metal : public Material
{
    common::Vec3 albedo;
    double fuzz;

public:
    Metal(const common::Vec3& _albedo, double _fuzz);
    [[nodiscard]] Ray get_scattered(const Ray& ray_in, const common::Vec3& intersection,
                                    const common::Vec3& normal) const override;
    [[nodiscard]] common::Vec3 get_color(const Ray& ray_in, const common::Vec3& intersection,
                                         const common::Vec3& normal) const override;
    [[nodiscard]] common::Vec3 get_albedo() const;
    [[nodiscard]] double get_fuzz() const;
    [[nodiscard]] MaterialType get_type() const override { return MaterialType::METAL; }
};
} // namespace raytracer::raytracer
