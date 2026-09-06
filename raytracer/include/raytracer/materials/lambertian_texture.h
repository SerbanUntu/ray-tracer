#pragma once
#include "lambertian.h"
#include "cubemap.h"

namespace raytracer::raytracer
{
class LambertianTexture : public Lambertian
{
    Cubemap cm;

public:
    explicit LambertianTexture(Cubemap _cm);
    [[nodiscard]] common::Vec3 get_color(const Ray& ray_in, const common::Vec3& intersection,
                                         const common::Vec3& normal) const override;
    [[nodiscard]] const Cubemap& get_cubemap() const;
    [[nodiscard]] MaterialType get_type() const override { return MaterialType::LAMBERTIAN_TEXTURE; }
};
} // namespace raytracer::raytracer
