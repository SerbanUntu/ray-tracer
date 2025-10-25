#include "lambertian_texture.h"

LambertianTexture::LambertianTexture(Cubemap _cm) : cm(_cm) {}

Vec3 LambertianTexture::get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	return cm.get_color_at_point(normal);
}

const Cubemap& LambertianTexture::get_cubemap() const { return cm; }

std::string LambertianTexture::get_type() const { return "LambertianTexture"; }
