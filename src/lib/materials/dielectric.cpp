#include "dielectric.h"
#include <math.h>

Dielectric::Dielectric(double eta) : refractive_index(eta) {}

Ray Dielectric::get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	double cosine = ray_in.direction.to_normalized() * normal.to_normalized();

	double eta_in = AIR_REFRACTIVE_INDEX;
	double eta_out = refractive_index;
	double ratio = eta_in / eta_out;

	// Ray comes from outside the object
	if (cosine > 0) {
		ratio = 1. / ratio;
	}

	Vec3 uv = ray_in.direction.to_normalized();
	Vec3 n = normal.to_normalized();

	double cos = std::min(-uv * n, 1.);
	double sin = std::sqrt(1. - std::pow(cos, 2));

	// Cannot refract
	if (sin * ratio > 1.0) {
		return Ray(intersection, reflect(ray_in.direction, normal));
	}

	return Ray(intersection, refract(ray_in.direction, normal, ratio));
}

Vec3 Dielectric::get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const {
	return Vec3(1, 1, 1);
}

double Dielectric::get_refractive_index() const {
	return refractive_index;
}

std::string Dielectric::get_type() const { return "Dielectric"; }

double Dielectric::AIR_REFRACTIVE_INDEX = 1.;
