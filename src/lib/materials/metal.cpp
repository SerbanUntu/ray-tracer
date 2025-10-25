#include "metal.h"
#include "util/random_utils.h"

Metal::Metal(Vec3 _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz) {}

Ray Metal::get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	return Ray(intersection, (reflect(ray_in.direction, normal) + random_unit() * fuzz).to_normalized());
}

Vec3 Metal::get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	return albedo;
}

Vec3 Metal::get_albedo() const {
	return albedo;
}

double Metal::get_fuzz() const {
	return fuzz;
}

std::string Metal::get_type() const { return "Metal"; }
