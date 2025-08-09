#pragma once
#include "util/vec3.h"
#include "materials/material.h"

class Object {
private:
	const Material* mat;
public:
	Object(const Material* _mat) : mat(_mat) {}
	const Material* get_material() const { return mat; }
	virtual double ray_intersection(Ray r) const = 0;
	virtual Vec3 get_normal(Vec3 point) const = 0;
};
