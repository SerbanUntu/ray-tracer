#pragma once
#include <string>
#include "lambertian.h"
#include "cubemap.h"

class LambertianTexture : public Lambertian {
private:
	Cubemap cm;
public:
	LambertianTexture(Cubemap _cm);
	Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const override;
	const Cubemap& get_cubemap() const;
	std::string get_type() const override;
};
