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

class Material
{
public:
    virtual ~Material() = default;
    static common::Vec3 reflect(const common::Vec3& dir_in, const common::Vec3& normal);
    static common::Vec3 refract(const common::Vec3& dir_in, const common::Vec3& normal, double refractive_index);
    [[nodiscard]] virtual Ray get_scattered(const Ray& ray_in, const common::Vec3& intersection,
                                            const common::Vec3& normal) const = 0;
    [[nodiscard]] virtual common::Vec3 get_color(const Ray& ray_in, const common::Vec3& intersection,
                                                 const common::Vec3& normal) const = 0;
    [[nodiscard]] virtual MaterialType get_type() const = 0;
};
} // namespace raytracer::raytracer
