#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstddef>
#include <cstdint>
#include <random>
#include <memory>
#include "image.h"
#include "floor.h"
#include "sphere.h"
#include "util/vec3.h"
#include "util/random_utils.h"
#include "materials/lambertian.h"
#include "materials/lambertian_texture.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
#include "cubemap.h"
#include "camera.h"

// Scene config
const Vec3 BACKGROUND_COLOR = Vec3(0.39, 0.582, 0.9258);
const Vec3 SKY_COLOR = Vec3(1, 1, 1);
const int COLOR_CHANNELS = 256; // Maximum 256
const std::string FILENAME = "my_file.bmp";

const Camera camera = CameraBuilder::create()
->with_screen_dimensions(800, 800)
->with_rays_per_pixel(4)
->with_max_recursion_depth(50)
->build();

// Effects
const bool IS_SHADING_THRESHOLDED = false;
const double THRESHOLDED_SHADING_DEGREES = 4.0;
const bool IS_GRAYSCALE = false;

// Scene
const auto YELLOW = Lambertian(Vec3(1, 1, 0));
const auto STEEL = Metal(Vec3(0.4, 0.5, 0.6), 0.1);
const auto SOLID_GLASS = Dielectric(1.5);
const auto AIR_BUBBLE = Dielectric(1. / 1.33);
const auto FLOOR = Metal(Vec3(0.4, 0.4, 0.8), 0.6);

const Cubemap cm = Cubemap(
	Image(Vec3(1, 0, 0)),
	Image(Vec3(0, 1, 0)),
	Image(Vec3(0, 0, 1)),
	Image(Vec3(1, 1, 0)),
	Image(Vec3(0, 1, 1)),
	Image(Vec3(1, 0, 1))
);

const Cubemap cm2 = Cubemap(
	std::string("texture.bmp"),
	50,
	50,
	256,
	false
);

const auto TEXTURED = LambertianTexture(cm2);

const std::array<std::unique_ptr<Object>, 3> OBJECTS = {
	std::make_unique<Sphere>(Vec3(6, -5, -20), 6, &YELLOW),
	std::make_unique<Sphere>(Vec3(-6, -5, -20), 6, &STEEL),
	std::make_unique<Floor>(-11, &FLOOR),
};

static Vec3 shade(const Object& o, Vec3 intersection, Ray r, int depth, int max_depth);

static Vec3 trace(Ray r, int depth, int max_depth) {
	double min_depth = DBL_MAX;
	Vec3 top_color = (depth == 0) ? BACKGROUND_COLOR : SKY_COLOR;
	if (depth > max_depth) return Vec3::ZERO;

	for (const auto& oPtr : OBJECTS) {
		const Object& o = *oPtr;
		const double t = o.ray_intersection(r);
		const Vec3 intersection = r.origin + r.direction * t;
		if (t < min_depth && t > 0.001 && intersection.z < -1) {
			min_depth = t;
			top_color = shade(o, intersection, r, depth, max_depth);
		}
	}

	return top_color;
}

static Vec3 shade(const Object& o, Vec3 intersection, Ray r, int depth, int max_depth) {
	if (depth > max_depth) return Vec3::ZERO;

	const Material* mat = o.get_material();

	Vec3 normal = o.get_normal(intersection);
	Vec3 rec = trace(mat->get_scattered(r, intersection, normal), depth + 1, max_depth);
	Vec3 col = mat->get_color(r, intersection, normal);

	return Vec3(rec.x * col.x, rec.y * col.y, rec.z * col.z);
}

int main()
{
	Image img = Image(camera.screen_width_pixels, camera.screen_height_pixels, COLOR_CHANNELS, IS_GRAYSCALE);

	for (int i = 0; i < camera.screen_width_pixels; i++) {
		for (int j = 0; j < camera.screen_height_pixels; j++) {

			Vec3 pixel_color = Vec3(0, 0, 0);

			for (int k = 0; k < camera.rays_per_pixel; k++) {
				const Ray current_ray = camera.compute_ray_for_pixel(Pixel(i, j));
				pixel_color += trace(current_ray, 0, camera.max_recursion_depth);

			}

			img.draw(i, j, pixel_color / camera.rays_per_pixel);
		}
	}
	img.generateBmp(FILENAME);
	return 0;
}
