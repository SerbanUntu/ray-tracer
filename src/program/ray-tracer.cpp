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

static std::uniform_real_distribution<> offset_dist(-.5, .5);

// Scene config
const int LEFT = -1;
const int RIGHT = 1;
const int BOTTOM = -1;
const int TOP = 1;
const double FOCAL_LENGTH = 1;
const Vec3 CAMERA_ORIGIN = Vec3(0, 0, 0);
const Vec3 CAMERA_DIRECTION = Vec3(0, 0, -1);
const Vec3 WORLD_UP = Vec3(0, 1, 0);
const int WIDTH_PIXELS = 800;
const int HEIGHT_PIXELS = 800;
const ViewType VIEW_TYPE = PERSPECTIVE;
const Vec3 BACKGROUND_COLOR = Vec3(0.39, 0.582, 0.9258);
const Vec3 SKY_COLOR = Vec3(1, 1, 1);
const int COLOR_CHANNELS = 256; // Maximum 256
const std::string FILENAME = "my_file.bmp";
const int RAYS_PER_PIXEL = 4;
const int MAX_DEPTH = 50;

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

static Vec3 shade(const Object& o, Vec3 intersection, Ray r, int depth);

static Ray compute_ray_for_pixel(Pixel p) {

	//const Vec3 forward = CAMERA_DIRECTION.to_normalized();
	//const Vec3 right = forward.cross(WORLD_UP).to_normalized();
	//const Vec3 up = right.cross(forward).to_normalized();

	//const double CAMERA_LEFT = static_cast<double>(LEFT * WIDTH_PIXELS) / HEIGHT_PIXELS;
	//const double CAMERA_RIGHT = static_cast<double>(RIGHT * WIDTH_PIXELS) / HEIGHT_PIXELS;

	//// Random sampling is disabled when there is a single ray per pixel
	//const double RANDOM_X = (double)p.x + ((RAYS_PER_PIXEL > 1) ? offset_dist(gen) : 0);
	//const double RANDOM_Y = (double)p.y + ((RAYS_PER_PIXEL > 1) ? offset_dist(gen) : 0);

	//const double u = CAMERA_LEFT + (RANDOM_Y / (double)WIDTH_PIXELS) * (CAMERA_RIGHT - CAMERA_LEFT);
	//const double v = TOP + (RANDOM_X / (double)HEIGHT_PIXELS) * (BOTTOM - TOP);

	//if (VIEW_TYPE == PERSPECTIVE) {
	//	Vec3 ray_direction = right * u + up * v + forward * FOCAL_LENGTH;
	//	return Ray(CAMERA_ORIGIN.to_normalized(), ray_direction.to_normalized());
	//}
	//else if (VIEW_TYPE == ORTHOGRAPHIC) {
	//	Vec3 ray_origin = right * u + up * v + CAMERA_ORIGIN;
	//	return Ray(ray_origin.to_normalized(), forward.to_normalized());
	//}
	const double CAMERA_LEFT = static_cast<double>(LEFT * WIDTH_PIXELS) / HEIGHT_PIXELS;
	const double CAMERA_RIGHT = static_cast<double>(RIGHT * WIDTH_PIXELS) / HEIGHT_PIXELS;

	// Random sampling is disabled when there is a single ray per pixel
	const double RANDOM_X = (double)p.x + ((RAYS_PER_PIXEL > 1) ? offset_dist(gen) : 0);
	const double RANDOM_Y = (double)p.y + ((RAYS_PER_PIXEL > 1) ? offset_dist(gen) : 0);

	const double u = CAMERA_LEFT + (RANDOM_Y / (double)WIDTH_PIXELS) * (CAMERA_RIGHT - CAMERA_LEFT);
	const double v = TOP + (RANDOM_X / (double)HEIGHT_PIXELS) * (BOTTOM - TOP);

	if (VIEW_TYPE == PERSPECTIVE) {
		Vec3 ray_direction = Vec3(u, v, -FOCAL_LENGTH);
		return Ray(CAMERA_ORIGIN, ray_direction);
	}
	else if (VIEW_TYPE == ORTHOGRAPHIC) {
		Vec3 ray_origin = Vec3(u, v, 0) + CAMERA_ORIGIN;
		return Ray(ray_origin, Vec3(0, 0, -1));
	}
}

static Vec3 trace(Ray r, int depth) {
	double min_depth = DBL_MAX;
	Vec3 top_color = (depth == 0) ? BACKGROUND_COLOR : SKY_COLOR;
	if (depth > MAX_DEPTH) return Vec3::ZERO;

	for (const auto& oPtr : OBJECTS) {
		const Object& o = *oPtr;
		const double t = o.ray_intersection(r);
		const Vec3 intersection = r.origin + r.direction * t;
		if (t < min_depth && t > 0.001 && intersection.z < -1) {
			min_depth = t;
			top_color = shade(o, intersection, r, depth);
		}
	}

	return top_color;
}

static Vec3 shade(const Object& o, Vec3 intersection, Ray r, int depth) {
	if (depth > MAX_DEPTH) return Vec3::ZERO;

	const Material* mat = o.get_material();

	Vec3 normal = o.get_normal(intersection);
	Vec3 rec = trace(mat->get_scattered(r, intersection, normal), depth + 1);
	Vec3 col = mat->get_color(r, intersection, normal);

	return Vec3(rec.x * col.x, rec.y * col.y, rec.z * col.z);
}

int main()
{
	Image img = Image(WIDTH_PIXELS, HEIGHT_PIXELS, COLOR_CHANNELS, IS_GRAYSCALE);

	for (int i = 0; i < HEIGHT_PIXELS; i++) {
		for (int j = 0; j < WIDTH_PIXELS; j++) {

			Vec3 pixel_color = Vec3::ZERO;

			for (int k = 0; k < RAYS_PER_PIXEL; k++) {
				const Ray current_ray = compute_ray_for_pixel(Pixel(i, j));
				pixel_color += trace(current_ray, 0);

			}

			img.draw(i, j, pixel_color / RAYS_PER_PIXEL);
		}
	}
	img.generateBmp(FILENAME);
	return 0;
}
