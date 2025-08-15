#pragma once
#include "object.h"

class Floor : public Object {
private:
	double y;
public:
	Floor(double _y, const Material* _mat) : y(_y), Object(_mat) {}
	double get_y() const { return y; }
	double ray_intersection(Ray r) const override {
		return (y - r.origin.y) / r.direction.y;
	}
	Vec3 get_normal(Vec3 point) const override {
		return Vec3(0, 1, 0);
	}
};
