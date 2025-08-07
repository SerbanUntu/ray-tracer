#pragma once
#include <math.h>
#include "util/vec3.h"
#include "ray-tracer.h"

class Sphere {
private:
	Vec3 center;
	double radius;
	Color color;

public:
	Sphere(Vec3 _center, double _radius, Color _color) : center(_center), radius(_radius), color(_color) {
	}
	inline Color get_color() const { return color; }
	inline double ray_intersection(Ray r) const {
		const double A = r.direction * r.direction;
		const double B = r.direction * (r.origin - center) * 2;
		const double C = (r.origin - center) * (r.origin - center) - radius * radius;
		const double delta = B * B - 4 * A * C;
		if (delta < 0) return -1.0;
		const double sqrt_delta = sqrt(delta);
		const double t1 = (-B - sqrt_delta) / (2 * A);
		const double t2 = (-B + sqrt_delta) / (2 * A);
		return t1 >= t2 ? t2 : t1;
	}
};
