#pragma once
#include "object.h"

class Floor : public Object {
private:
	double y;
public:
	Floor(double _y, std::unique_ptr<const Material> _mat);
	double get_y() const;
	double ray_intersection(Ray r) const override;
	Vec3 get_normal(Vec3 point) const override;
	std::string get_type() const override;
};
