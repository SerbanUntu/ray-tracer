#pragma once
#include <string>
#include "material.h"
#include "util/vec3.h"

class Lambertian : public Material {
private:
	Vec3 albedo;
public:
	Lambertian();
	Lambertian(Vec3 _albedo);
	Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	Vec3 get_albedo() const;
	std::string get_type() const override;
};
