#pragma once
#include "material.h"

namespace raytracer::raytracer
{
/**
 * A transparent material, like glass or water, which refracts rays passing through it
 * (or reflects them when refraction is impossible).
 */
class Dielectric : public Material
{
    static constexpr double AIR_REFRACTIVE_INDEX = 1.0;
    double refractive_index;

  public:
    /**
     * Creates a dielectric material with the given refractive index.
     *
     * @param eta The refractive index of the material, relative to air.
     */
    explicit Dielectric(double eta);

    [[nodiscard]] Ray get_scattered(const Ray &ray_in, const common::Vec3 &intersection,
                                    const common::Vec3 &normal) const override;

    [[nodiscard]] common::Vec3 get_color(const Ray &ray_in, const common::Vec3 &intersection,
                                         const common::Vec3 &normal) const override;

    /**
     * Getter for the refractive index of the material.
     *
     * @return The refractive index of the material.
     */
    [[nodiscard]] double get_refractive_index() const;

    [[nodiscard]] MaterialType get_type() const override
    {
        return MaterialType::DIELECTRIC;
    }
};
} // namespace raytracer::raytracer
