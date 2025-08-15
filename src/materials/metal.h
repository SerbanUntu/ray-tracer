#pragma once
#include "material.h"
#include "../util/random_utils.h"

class Metal : public Material {
private:
	Vec3 albedo;
	double fuzz;
public:
	Metal(Vec3 _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz) {}
	Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		return Ray(intersection, (reflect(ray_in.direction, normal) + random_unit() * fuzz).to_normalized());
	}
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		return albedo;
	}
};
