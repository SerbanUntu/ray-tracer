#pragma once
#include "util/vec3.h"
#include "ray-tracer.h"

class Object {
private:
	Material mat;
public:
	Object(Material _mat) : mat(_mat) {}
	Material get_material() const { return mat; }
	virtual double ray_intersection(Ray r) const = 0;
	virtual Vec3 get_normal(Vec3 point) const = 0;
};
