#pragma once
#include "../util/vec3.h"
#include "../ray-tracer.h"

class Material {
public:
	inline static Vec3 reflect(Vec3 dir_in, Vec3 normal) {
		Vec3 v = dir_in.to_normalized();
		Vec3 n = normal.to_normalized();

		return (v + n * 2).to_normalized();
	}
	inline static Vec3 refract(Vec3 dir_in, Vec3 normal, double eta_in, double eta_out) {
		Vec3 v = dir_in.to_normalized();
		Vec3 n = normal.to_normalized();

		static double cosine = std::min(-v * n, 1.);

		Vec3 r_perp = (v + n * cosine) * (eta_in / eta_out);

		Vec3 r_para = -n * std::sqrt(std::abs(1 - std::pow(r_perp.length(), 2)));
		return (r_perp + r_para).to_normalized();
	}
	virtual Ray get_scattered(Ray ray_in, Vec3 intersection, Vec3 normal) const = 0;
	virtual Vec3 get_color(Ray ray_in, Vec3 intersection, Vec3 normal) const = 0;
};
