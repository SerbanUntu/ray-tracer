#include "ray-tracer.h"
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

// Random utils
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dist(-.5, .5);

// Scene config
const int LEFT = -1;
const int RIGHT = 1;
const int BOTTOM = -1;
const int TOP = 1;
const double FOCAL_LENGTH = 1;
const Vec3 ORIGIN = Vec3::ZERO;
const int WIDTH_PIXELS = 800;
const int HEIGHT_PIXELS = 800;
const ViewType VIEW_TYPE = PERSPECTIVE;
const Vec3 LIGHT_DIRECTION = Vec3(1, -1, 0);
const double LIGHT_INTENSITY = 1;
const Vec3 AMBIENT_COLOR = Vec3(0, 0.1, 0.3);
const double AMBIENT_LIGHT_INTENSITY = 0.2;
const Vec3 BACKGROUND_COLOR = Vec3(0.39, 0.582, 0.9258);
const int COLOR_CHANNELS = 256; // Maximum 256
const std::string FILENAME = "my_file.bmp";
const int RAYS_PER_PIXEL = 4;
const int MAX_DEPTH = 50;

// Effects
const bool IS_SHADING_THRESHOLDED = false;
const double THRESHOLDED_SHADING_DEGREES = 4.0;
const bool IS_GRAYSCALE = false;

// Scene
const Material YELLOW = Material(Vec3(1, 1, 0), Vec3(1, 1, 1), 10);
const Material CYAN = Material(Vec3(0, 1, 1), Vec3(1, 1, 1), 100);
const Material FLOOR = Material(Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1000);

const std::array<std::unique_ptr<Object>, 3> OBJECTS = {
	std::make_unique<Sphere>(Vec3(6, -5, -20), 6, YELLOW),
	std::make_unique<Sphere>(Vec3(-6, -5, -20), 6, CYAN),
	std::make_unique<Floor>(-11, FLOOR),
};

static Vec3 shade(const Object& o, Vec3 intersection, Vec3 ray_direction, int depth);

static Vec3 random_unit() {
	return Vec3(dist(gen) * 2, dist(gen) * 2, dist(gen) * 2).to_normalized();
}

static Ray compute_ray_for_pixel(Pixel p) {

	const double CAMERA_LEFT = static_cast<double>(LEFT * WIDTH_PIXELS) / HEIGHT_PIXELS;
	const double CAMERA_RIGHT = static_cast<double>(RIGHT * WIDTH_PIXELS) / HEIGHT_PIXELS;

	// Random sampling is disabled when there is a single ray per pixel
	const double RANDOM_X = (double)p.x + ((RAYS_PER_PIXEL > 1) ? dist(gen) : 0);
	const double RANDOM_Y = (double)p.y + ((RAYS_PER_PIXEL > 1) ? dist(gen) : 0);

	const double u = CAMERA_LEFT + (RANDOM_Y / (double)WIDTH_PIXELS) * (CAMERA_RIGHT - CAMERA_LEFT);
	const double v = TOP + (RANDOM_X / (double)HEIGHT_PIXELS) * (BOTTOM - TOP);

	if (VIEW_TYPE == PERSPECTIVE) {
		Vec3 ray_direction = Vec3(u, v, -FOCAL_LENGTH);
		return Ray(ORIGIN, ray_direction);
	}
	else if (VIEW_TYPE == ORTHOGRAPHIC) {
		Vec3 ray_origin = Vec3(u, v, 0) + ORIGIN;
		return Ray(ray_origin, Vec3(0, 0, -1));
	}
}

static Vec3 trace(Ray r, int depth) {
	double min_depth = DBL_MAX;
	Vec3 top_color = (depth == 0) ? BACKGROUND_COLOR : Vec3(1, 1, 1);
	if (depth > MAX_DEPTH) return Vec3::ZERO;

	for (const auto& oPtr : OBJECTS) {
		const Object& o = *oPtr;
		const double t = o.ray_intersection(r);
		const Vec3 intersection = r.origin + r.direction * t;
		if (t < min_depth && t > 0.001 && intersection.z < -1) {
			min_depth = t;
			top_color = shade(o, intersection, r.direction, depth);
		}
	}

	return top_color;
}

static Vec3 shade(const Object& o, Vec3 intersection, Vec3 ray_direction, int depth) {
	if (depth > MAX_DEPTH) return Vec3::ZERO;

	Material mat = o.get_material();
	Vec3 d = mat.diffuse;

	Vec3 normal = o.get_normal(intersection);
	Vec3 rec = trace(Ray(intersection, (normal + random_unit()).to_normalized()), depth + 1);
	return Vec3(rec.x * d.x, rec.y * d.y, rec.z * d.z);
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
