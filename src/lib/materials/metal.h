#pragma once
#include <string>
#include "material.h"

class Metal : public Material
{
    Vec3 albedo;
    double fuzz;

public:
    Metal(const Vec3& _albedo, double _fuzz);
    [[nodiscard]] Ray get_scattered(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] Vec3 get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const override;
    [[nodiscard]] Vec3 get_albedo() const;
    [[nodiscard]] double get_fuzz() const;
    [[nodiscard]] std::string get_type() const override;
};
