#pragma once
#include <random>
#include "util/vec3.h"

static std::uniform_real_distribution<> offset_dist(-.5, .5);

enum ViewType {
	ORTHOGRAPHIC,
	PERSPECTIVE
};

struct Ray {
	Vec3 origin;
	Vec3 direction;
};

struct Pixel {
	int x;
	int y;
};

class Camera {
public:
	double screen_left_coord = -1.;
	double screen_right_coord = 1.;
	double screen_bottom_coord = -1.;
	double screen_top_coord = 1.;
	double focal_length = 1.;
	Vec3 origin = Vec3::ZERO;
	Vec3 direction = Vec3(0, 0, -1);
	Vec3 world_up = Vec3(0, 1, 0);
	int screen_width_pixels = 1920;
	int screen_height_pixels = 1080;
	ViewType view_type = PERSPECTIVE;
	int color_channels = 256;
	int rays_per_pixel = 4;
	int max_recursion_depth = 32;

	Camera();
	Camera(
		double _screen_left_coord,
		double _screen_right_coord,
		double _screen_bottom_coord,
		double _screen_top_coord,
		double _focal_length,
		Vec3 _origin,
		Vec3 _direction,
		Vec3 _world_up,
		int _screen_width_pixels,
		int _screen_height_pixels,
		ViewType _view_type,
		int _color_channels,
		int _rays_per_pixel,
		int _max_recursion_depth
	);
	Ray compute_ray_for_pixel(Pixel p) const;
};

class CameraBuilder {
private:
	Camera camera;
	CameraBuilder();
public:
	static CameraBuilder* create();
	CameraBuilder* with_screen_dimensions(int width, int height);
	CameraBuilder* with_focal_length(double focal_length);
	CameraBuilder* with_origin(Vec3 origin);
	CameraBuilder* with_direction(Vec3 direction);
	CameraBuilder* with_world_up(Vec3 direction);
	CameraBuilder* with_virtual_screen_boundaries(double left, double top, double right, double bottom);
	CameraBuilder* with_view_type(ViewType view_type);
	CameraBuilder* with_color_channels(int color_channels);
	CameraBuilder* with_rays_per_pixel(int rays_per_pixel);
	CameraBuilder* with_max_recursion_depth(int max_recursion_depth);
	Camera build() const;
};

