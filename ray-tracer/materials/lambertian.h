#pragma once
#include "material.h"
#include "../util/random_utils.h"

class Lambertian : public Material {
private:
	Vec3 albedo;
public:
	Lambertian() : albedo(Vec3::ZERO) {}
	Lambertian(Vec3 _albedo) : albedo(_albedo) {}
	Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		return Ray(intersection, (normal + random_unit()).to_normalized());
	}
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		return albedo;
	}
};
