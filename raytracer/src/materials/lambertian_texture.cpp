#include "raytracer/materials/lambertian_texture.h"
#include <utility>

namespace raytracer::raytracer
{
LambertianTexture::LambertianTexture(Cubemap _cm) : cm(std::move(_cm))
{
}

common::Vec3 LambertianTexture::get_color(const Ray& ray_in, const common::Vec3& intersection,
                                          const common::Vec3& normal) const
{
    return cm.get_color_at_point(normal);
}

const Cubemap& LambertianTexture::get_cubemap() const { return cm; }
} // namespace raytracer::raytracer
