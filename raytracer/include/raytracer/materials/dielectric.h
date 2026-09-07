#pragma once
#include "material.h"

namespace raytracer::raytracer
{
class Dielectric : public Material
{
    static constexpr double AIR_REFRACTIVE_INDEX = 1.0;
    double refractive_index;

public:
    explicit Dielectric(double eta);
    [[nodiscard]] Ray get_scattered(const Ray& ray_in, const common::Vec3& intersection,
                                    const common::Vec3& normal) const override;
    [[nodiscard]] common::Vec3 get_color(const Ray& ray_in, const common::Vec3& intersection,
                                         const common::Vec3& normal) const override;
    [[nodiscard]] double get_refractive_index() const;
    [[nodiscard]] MaterialType get_type() const override { return MaterialType::DIELECTRIC; }
};
} // namespace raytracer::raytracer
