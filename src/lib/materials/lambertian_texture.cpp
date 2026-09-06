#include "lambertian_texture.h"
#include <utility>

LambertianTexture::LambertianTexture(Cubemap _cm) : cm(std::move(_cm))
{
}

Vec3 LambertianTexture::get_color(const Ray& ray_in, const Vec3& intersection, const Vec3& normal) const
{
    return cm.get_color_at_point(normal);
}

const Cubemap& LambertianTexture::get_cubemap() const { return cm; }

std::string LambertianTexture::get_type() const { return "LambertianTexture"; }
