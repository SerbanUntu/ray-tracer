#pragma once
#include <string>
#include "material.h"

class Dielectric : public Material
{
    static double AIR_REFRACTIVE_INDEX;
    double refractive_index;

public:
    explicit Dielectric(double eta);
    [[nodiscard]] Ray get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] Vec3 get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] double get_refractive_index() const;
    [[nodiscard]] std::string get_type() const override;
};
