#include "material.h"
#include <math.h>

Vec3 Material::reflect(Vec3 dir_in, Vec3 normal) {
	Vec3 v = dir_in.to_normalized();
	Vec3 n = normal.to_normalized();

	return (v - n * 2 * (v * n)).to_normalized();
}

Vec3 Material::refract(Vec3 dir_in, Vec3 normal, double refractive_index) {
	Vec3 v = dir_in.to_normalized();
	Vec3 n = normal.to_normalized();

	double cosine = std::min(-v * n, 1.);

	Vec3 r_perp = (v + n * cosine) * refractive_index;

	Vec3 r_para = -n * std::sqrt(std::abs(1 - std::pow(r_perp.length(), 2)));
	return (r_perp + r_para).to_normalized();
}
