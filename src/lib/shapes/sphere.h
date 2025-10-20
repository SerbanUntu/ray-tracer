#pragma once
#include <math.h>
#include "util/vec3.h"
#include "camera.h"
#include "object.h"

class Sphere : public Object {
private:
	Vec3 center;
	double radius;

public:
	Sphere(Vec3 _center, double _radius, std::unique_ptr<const Material> _mat) : center(_center), radius(_radius), Object(std::move(_mat)) {}
	Vec3 get_center() const { return center; }
	double get_radius() const { return radius; }
	double ray_intersection(Ray r) const override {
		const double A = r.direction * r.direction;
		const double B = r.direction * (r.origin - center) * 2;
		const double C = (r.origin - center) * (r.origin - center) - radius * radius;
		const double delta = B * B - 4 * A * C;
		if (delta < 0) return -1.0;
		const double sqrt_delta = sqrt(delta);
		const double t1 = (-B - sqrt_delta) / (2 * A);
		const double t2 = (-B + sqrt_delta) / (2 * A);

		if (t1 >= 0) return t1;
		return t2;
	}
	Vec3 get_normal(Vec3 point) const override {
		return (point - center).to_normalized();
	}
	std::string get_type() const override { return "Sphere"; }
};
