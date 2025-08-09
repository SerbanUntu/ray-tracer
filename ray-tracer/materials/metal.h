#pragma once
#include "material.h"
#include "../util/random_utils.h"

class Metal : public Material {
private:
	double fuzz;
public:
	Metal(Vec3 _albedo, double _fuzz) : Material(_albedo), fuzz(_fuzz) {}
	Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		const Vec3 reflection = (ray_in.direction.to_normalized() + normal.to_normalized() * 2).to_normalized();
		return Ray(intersection, (reflection + random_unit() * fuzz).to_normalized());
	}
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		return albedo;
	}
};
