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
#include <nlohmann/json.hpp>
#include "image.h"
#include "scene.h"
#include "shapes/floor.h"
#include "shapes/sphere.h"
#include "util/vec3.h"
#include "util/random_utils.h"
#include "materials/lambertian.h"
#include "materials/lambertian_texture.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
#include "materials/cubemap.h"
#include "camera.h"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vec3, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Camera, screen_left_coord, screen_right_coord, screen_bottom_coord, screen_top_coord, focal_length, origin, direction, world_up, screen_width_pixels, screen_height_pixels, view_type, color_channels, rays_per_pixel, max_recursion_depth)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RayTracerSceneConfig, background_color, sky_color, camera, objects, is_shading_thresholded, thresholded_shading_degrees, is_grayscale, output_path)
NLOHMANN_JSON_SERIALIZE_ENUM(ViewType, {
	{ViewType::ORTHOGRAPHIC, "orthographic"},
	{ViewType::PERSPECTIVE, "perspective"}
})

static Vec3 shade(const Object& o, Vec3 intersection, Ray r, int depth, int max_depth, const RayTracerSceneConfig& scene);

static Vec3 trace(Ray r, int depth, int max_depth, const RayTracerSceneConfig& scene) {
	double min_depth = DBL_MAX;
	Vec3 top_color = (depth == 0) ? scene.background_color : scene.sky_color;
	if (depth > max_depth) return Vec3::ZERO;

	for (const auto& oPtr : scene.objects) {
		const Object& o = *oPtr;
		const double t = o.ray_intersection(r);
		const Vec3 intersection = r.origin + r.direction * t;
		if (t < min_depth && t > 0.001 && intersection.z < -1) {
			min_depth = t;
			top_color = shade(o, intersection, r, depth, max_depth, scene);
		}
	}

	return top_color;
}

static Vec3 shade(const Object& o, Vec3 intersection, Ray r, int depth, int max_depth, const RayTracerSceneConfig& scene) {
	if (depth > max_depth) return Vec3::ZERO;

	const Material* mat = o.get_material();

	Vec3 normal = o.get_normal(intersection);
	Vec3 rec = trace(mat->get_scattered(r, intersection, normal), depth + 1, max_depth, scene);
	Vec3 col = mat->get_color(r, intersection, normal);

	return Vec3(rec.x * col.x, rec.y * col.y, rec.z * col.z);
}

int main()
{

	std::vector<std::unique_ptr<Object>> objects;
	objects.emplace_back(
		std::make_unique<Sphere>(
			Vec3(6, -5, -20),
			6,
			std::make_unique<const Lambertian>(Vec3(1, 1, 0))
		)
	);
	objects.emplace_back(
		std::make_unique<Sphere>(
			Vec3(-6, -5, -20),
			6,
			std::make_unique<const Metal>(Vec3(0.4, 0.5, 0.6), 0.1)
		)
	);
	objects.emplace_back(
		std::make_unique<Sphere>(
			Vec3(0, 10, -120),
			20,
			std::make_unique<const LambertianTexture>(
				Cubemap(
					Image(Vec3(1, 0, 0)),
					Image(Vec3(0, 1, 0)),
					Image(Vec3(0, 0, 1)),
					Image(Vec3(1, 1, 0)),
					Image(Vec3(0, 1, 1)),
					Image(Vec3(1, 0, 1))
				)
			)
		)
	);
	objects.emplace_back(
		std::make_unique<Floor>(
			-11,
			std::make_unique<const Metal>(Vec3(0.4, 0.4, 0.8), 0.6)
		)
	);

	// Scene config
	const RayTracerSceneConfig scene(
		Vec3(0.39, 0.582, 0.9258),
		Vec3(1, 1, 1),
		Camera(
			-1.,
			1.,
			-1.,
			1.,
			1.,
			Vec3::ZERO,
			Vec3(0, 0, -1),
			Vec3(0, 1, 0),
			800,
			800,
			PERSPECTIVE,
			256,
			4,
			50
		),
		std::move(objects),
		false,
		4.0,
		false,
		"ray-tracer.bmp"
	);

	Image img = Image(
		scene.camera.screen_width_pixels,
		scene.camera.screen_height_pixels,
		scene.camera.color_channels,
		scene.is_grayscale
	);

	for (int i = 0; i < scene.camera.screen_width_pixels; i++) {
		for (int j = 0; j < scene.camera.screen_height_pixels; j++) {

			Vec3 pixel_color = Vec3(0, 0, 0);

			for (int k = 0; k < scene.camera.rays_per_pixel; k++) {
				const Ray current_ray = scene.camera.compute_ray_for_pixel(Pixel(i, j));
				pixel_color += trace(current_ray, 0, scene.camera.max_recursion_depth, scene);

			}

			img.draw(i, j, pixel_color / scene.camera.rays_per_pixel);
		}
	}
	img.generateBmp(scene.output_path);
	return 0;
}
