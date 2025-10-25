#pragma once
#include <string>
#include "material.h"

class Metal : public Material {
private:
	Vec3 albedo;
	double fuzz;
public:
	Metal(Vec3 _albedo, double _fuzz);
	Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	Vec3 get_albedo() const;
	double get_fuzz() const;
	std::string get_type() const override;
};
