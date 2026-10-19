#pragma once
#include "lambertian.h"
#include "cubemap.h"

namespace raytracer::raytracer
{
/**
 * A matte material whose color is sampled from a cubemap texture instead of a single albedo.
 */
class LambertianTexture : public Lambertian
{
    Cubemap cm;

  public:
    /**
     * Creates a textured Lambertian material from the given cubemap.
     *
     * @param _cm The cubemap from which the color of the material is sampled.
     */
    explicit LambertianTexture(Cubemap _cm);

    [[nodiscard]] common::Vec3 get_color(const Ray &ray_in, const common::Vec3 &intersection,
                                         const common::Vec3 &normal) const override;

    /**
     * Returns a const reference to the cubemap of the material.
     *
     * @return A const reference to the cubemap of the material.
     */
    [[nodiscard]] const Cubemap &get_cubemap() const;

    [[nodiscard]] MaterialType get_type() const override
    {
        return MaterialType::LAMBERTIAN_TEXTURE;
    }
};
} // namespace raytracer::raytracer
