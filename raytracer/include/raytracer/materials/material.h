#pragma once
#include "common/util/vec3.h"
#include "../camera.h"

namespace raytracer::raytracer
{
enum class MaterialType : uint8_t
{
    DIELECTRIC,
    LAMBERTIAN,
    LAMBERTIAN_TEXTURE,
    METAL
};

/**
 * The surface of an object, which determines how rays bounce off of it and which color they pick up.
 */
class Material
{
  public:
    virtual ~Material() = default;

    /**
     * Reflects a direction vector about a surface normal, like a mirror.
     *
     * @param dir_in The direction of the incoming ray.
     * @param normal The normal to the surface at the point of reflection.
     * @return The normalized direction of the reflected ray.
     */
    static common::Vec3 reflect(const common::Vec3 &dir_in, const common::Vec3 &normal);

    /**
     * Refracts a direction vector through a surface, according to Snell's law.
     *
     * @param dir_in The direction of the incoming ray.
     * @param normal The normal to the surface at the point of refraction.
     * @param refractive_index The ratio between the refractive index of the medium the ray leaves and that of the
     * medium it enters.
     * @return The normalized direction of the refracted ray.
     */
    static common::Vec3 refract(const common::Vec3 &dir_in, const common::Vec3 &normal, double refractive_index);

    /**
     * Computes the ray that leaves the surface after an incoming ray hits it.
     *
     * @param ray_in The incoming ray.
     * @param intersection The point where the incoming ray hits the surface.
     * @param normal The normal to the surface at the intersection point.
     * @return The scattered ray, starting at the intersection point.
     */
    [[nodiscard]] virtual Ray get_scattered(const Ray &ray_in, const common::Vec3 &intersection,
                                            const common::Vec3 &normal) const = 0;

    /**
     * Computes the color the surface contributes to a ray that hits it, which is multiplied with the color of the
     * scattered ray.
     *
     * @param ray_in The incoming ray.
     * @param intersection The point where the incoming ray hits the surface.
     * @param normal The normal to the surface at the intersection point.
     * @return The color of the surface at the intersection point.
     */
    [[nodiscard]] virtual common::Vec3 get_color(const Ray &ray_in, const common::Vec3 &intersection,
                                                 const common::Vec3 &normal) const = 0;

    /**
     * Returns the type of the material, used to identify the concrete class.
     *
     * @return The type of the material.
     */
    [[nodiscard]] virtual MaterialType get_type() const = 0;
};
} // namespace raytracer::raytracer
