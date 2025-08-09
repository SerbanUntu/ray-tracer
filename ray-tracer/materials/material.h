#pragma once
#include "../util/vec3.h"
#include "../ray-tracer.h"

class Material {
protected:
	Vec3 albedo;
public:
	Material(Vec3 _albedo) : albedo(_albedo) {}
	virtual Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const = 0;
	virtual Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const = 0;
};
