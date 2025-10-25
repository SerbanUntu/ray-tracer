#pragma once
#include <string>
#include "util/vec3.h"
#include "camera.h"

class Material {
public:
	static Vec3 reflect(Vec3 dir_in, Vec3 normal);
	static Vec3 refract(Vec3 dir_in, Vec3 normal, double refractive_index);
	virtual Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const = 0;
	virtual Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const = 0;
	virtual std::string get_type() const = 0;
};
