#pragma once
#include "lambertian.h"
#include "cubemap.h"

class LambertianTexture : public Lambertian {
private:
	Cubemap cm;
public:
	LambertianTexture(Cubemap _cm) : cm(_cm) {}
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override {
		return cm.get_color_at_point(normal);
	}
};
