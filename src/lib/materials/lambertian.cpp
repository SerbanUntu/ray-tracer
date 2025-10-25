#include "lambertian.h"
#include "util/random_utils.h"

Lambertian::Lambertian() : albedo(Vec3::ZERO) {}

Lambertian::Lambertian(Vec3 _albedo) : albedo(_albedo) {}

Ray Lambertian::get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	return Ray(intersection, (normal + random_unit()).to_normalized());
}

Vec3 Lambertian::get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	return albedo;
}

Vec3 Lambertian::get_albedo() const {
	return albedo;
}

std::string Lambertian::get_type() const { return "Lambertian"; }
