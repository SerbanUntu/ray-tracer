#include "floor.h"

Floor::Floor(double _y, std::unique_ptr<const Material> _mat) : y(_y), Object(std::move(_mat)) {}

double Floor::get_y() const { return y; }

double Floor::ray_intersection(Ray r) const {
	return (y - r.origin.y) / r.direction.y;
}

Vec3 Floor::get_normal(Vec3 point) const {
	return Vec3(0, 1, 0);
}

std::string Floor::get_type() const { return "Floor"; }
