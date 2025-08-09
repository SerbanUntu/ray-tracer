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
#include "image.h"
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
const Vec3 ORIGIN = Vec3(0, 0, 0);
const int WIDTH_PIXELS = 800;
const int HEIGHT_PIXELS = 800;
const ViewType VIEW_TYPE = PERSPECTIVE;
const Vec3 LIGHT_DIRECTION = Vec3(1, -1, 0);
const double LIGHT_INTENSITY = 1;
const Vec3 AMBIENT_COLOR = Vec3(0, 0.1, 0.3);
const double AMBIENT_LIGHT_INTENSITY = 0.2;
const Vec3 BACKGROUND_COLOR = Vec3(0, 0, 0);
const int COLOR_CHANNELS = 256; // Maximum 256
const std::string FILENAME = "my_file.bmp";
const int RAYS_PER_PIXEL = 100;

// Effects
const bool IS_SHADING_THRESHOLDED = false;
const double THRESHOLDED_SHADING_DEGREES = 4.0;
const bool IS_GRAYSCALE = false;

// Scene
const Material YELLOW = Material(Vec3(1, 1, 0), Vec3(1, 1, 1), 10);
const Material CYAN = Material(Vec3(0, 1, 1), Vec3(1, 1, 1), 100);
const Material MAGENTA = Material(Vec3(1, 0, 1), Vec3(1, 1, 1), 1000);

const std::array<Sphere, 3> OBJECTS = {
	Sphere(Vec3(0, -5, -20), 4, YELLOW),
	Sphere(Vec3(0, -20, -40), 20, CYAN),
	Sphere(Vec3(0, -20, -2000), 1500, MAGENTA),
};

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

static Vec3 shade(const Sphere s, Vec3 intersection, Vec3 ray_direction) {
	Material mat = s.get_material();

	Vec3 normal = (intersection - s.get_center()).to_normalized();
	Vec3 light = -LIGHT_DIRECTION.to_normalized();
	Vec3 view = -ray_direction;
	Vec3 half_vector = (view + light).to_normalized();

	double diffuse_cosine = std::max(0.0, normal * light);
	double specular_cosine = std::pow(std::max(0.0, normal * half_vector), mat.phong_exp);

	if (IS_SHADING_THRESHOLDED) {
		diffuse_cosine = static_cast<double>(static_cast<int>(
			diffuse_cosine * THRESHOLDED_SHADING_DEGREES))
			/ THRESHOLDED_SHADING_DEGREES;
		specular_cosine = static_cast<double>(static_cast<int>(
			specular_cosine * THRESHOLDED_SHADING_DEGREES))
			/ THRESHOLDED_SHADING_DEGREES;
	}

	Vec3 diffuse_component = mat.diffuse * LIGHT_INTENSITY * diffuse_cosine;
	Vec3 specular_component = mat.specular * LIGHT_INTENSITY * specular_cosine;
	Vec3 ambient_component = AMBIENT_COLOR * AMBIENT_LIGHT_INTENSITY;

	return diffuse_component + specular_component + ambient_component;
}

int main()
{
	Image img = Image(WIDTH_PIXELS, HEIGHT_PIXELS, COLOR_CHANNELS, IS_GRAYSCALE);

	for (int i = 0; i < HEIGHT_PIXELS; i++) {
		for (int j = 0; j < WIDTH_PIXELS; j++) {

			Vec3 pixel_color = Vec3(0, 0, 0);

			for (int k = 0; k < RAYS_PER_PIXEL; k++) {
				double min_depth = DBL_MAX;
				Vec3 top_color = BACKGROUND_COLOR;
				const Ray current_ray = compute_ray_for_pixel(Pixel(i, j));

				for (const Sphere s : OBJECTS) {
					const double t = s.ray_intersection(current_ray);
					const Vec3 intersection = current_ray.origin + current_ray.direction * t;
					if (t < min_depth && t > 0) {
						min_depth = t;
						top_color = shade(s, intersection, current_ray.direction);
					}
				}

				pixel_color += top_color / RAYS_PER_PIXEL;

			}

			img.draw(i, j, pixel_color);
		}
	}
	img.generateBmp(FILENAME);
	return 0;
}
