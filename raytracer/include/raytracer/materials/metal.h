#pragma once
#include "material.h"

namespace raytracer::raytracer
{
/**
 * A reflective material, which reflects rays like a mirror, optionally perturbed by a fuzz factor.
 */
class Metal : public Material
{
    common::Vec3 albedo;
    double fuzz;

  public:
    /**
     * Creates a metal material with the given properties.
     *
     * @param _albedo The color of the material.
     * @param _fuzz How much the reflected rays are randomly perturbed. Zero means a perfect mirror.
     */
    Metal(const common::Vec3 &_albedo, double _fuzz);

    [[nodiscard]] Ray get_scattered(const Ray &ray_in, const common::Vec3 &intersection,
                                    const common::Vec3 &normal) const override;

    [[nodiscard]] common::Vec3 get_color(const Ray &ray_in, const common::Vec3 &intersection,
                                         const common::Vec3 &normal) const override;

    /**
     * Getter for the albedo of the material.
     *
     * @return The albedo of the material.
     */
    [[nodiscard]] common::Vec3 get_albedo() const;

    /**
     * Getter for the fuzz factor of the material.
     *
     * @return The fuzz factor of the material.
     */
    [[nodiscard]] double get_fuzz() const;

    [[nodiscard]] MaterialType get_type() const override
    {
        return MaterialType::METAL;
    }
};
} // namespace raytracer::raytracer
