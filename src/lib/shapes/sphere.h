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
	Sphere(Vec3 _center, double _radius, std::unique_ptr<const Material> _mat);
	Vec3 get_center() const;
	double get_radius() const;
	double ray_intersection(Ray r) const override;
	Vec3 get_normal(Vec3 point) const override;
	std::string get_type() const override;
};
