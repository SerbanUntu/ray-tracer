#pragma once
#include <string>
#include <memory>
#include "util/vec3.h"
#include "materials/material.h"

class Object {
private:
	std::unique_ptr<const Material> mat;
public:
	Object(std::unique_ptr<const Material> _mat) : mat(std::move(_mat)) {}
	const Material* get_material() const { return mat.get(); }
	virtual double ray_intersection(Ray r) const = 0;
	virtual Vec3 get_normal(Vec3 point) const = 0;
	virtual std::string get_type() const = 0;
};
