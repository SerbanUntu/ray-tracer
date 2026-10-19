#pragma once
#include "material.h"
#include "common/util/vec3.h"

namespace raytracer::raytracer
{
/**
 * A matte material, which scatters rays in random directions around the surface normal.
 */
class Lambertian : public Material
{
    common::Vec3 albedo;

  public:
    /**
     * Creates a Lambertian material with a zero albedo (black).
     */
    Lambertian();

    /**
     * Creates a Lambertian material with the given albedo (color).
     *
     * @param _albedo The color of the material.
     */
    explicit Lambertian(const common::Vec3 &_albedo);

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

    [[nodiscard]] MaterialType get_type() const override
    {
        return MaterialType::LAMBERTIAN;
    }
};
} // namespace raytracer::raytracer
