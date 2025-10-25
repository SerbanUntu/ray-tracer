#pragma once
#include <string>
#include "material.h"

class Dielectric : public Material {
private:
	static double AIR_REFRACTIVE_INDEX;
	double refractive_index;
public:
	Dielectric(double eta);
	Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	double get_refractive_index() const;
	std::string get_type() const override;
};

