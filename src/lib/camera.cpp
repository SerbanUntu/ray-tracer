#include "camera.h"
#include <iostream>
#include "util/random_utils.h"

Camera::Camera() {}

Camera::Camera(
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
) :
	screen_left_coord(_screen_left_coord),
	screen_right_coord(_screen_right_coord),
	screen_bottom_coord(_screen_bottom_coord),
	screen_top_coord(_screen_top_coord),
	focal_length(_focal_length),
	origin(_origin),
	direction(_direction),
	world_up(_world_up),
	screen_width_pixels(_screen_width_pixels),
	screen_height_pixels(_screen_height_pixels),
	view_type(_view_type),
	color_channels(_color_channels),
	rays_per_pixel(_rays_per_pixel),
	max_recursion_depth(_max_recursion_depth) {
}

Ray Camera::compute_ray_for_pixel(Pixel p) const {

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

	const double CAMERA_LEFT = static_cast<double>(screen_left_coord * screen_width_pixels) / screen_height_pixels;
	const double CAMERA_RIGHT = static_cast<double>(screen_right_coord * screen_width_pixels) / screen_height_pixels;

	// Random sampling is disabled when there is a single ray per pixel
	const double RANDOM_X = (double)p.x + ((rays_per_pixel > 1) ? offset_dist(gen) : 0);
	const double RANDOM_Y = (double)p.y + ((rays_per_pixel > 1) ? offset_dist(gen) : 0);

	const double u = CAMERA_LEFT + (RANDOM_Y / (double)screen_width_pixels) * (CAMERA_RIGHT - CAMERA_LEFT);
	const double v = screen_top_coord + (RANDOM_X / (double)screen_height_pixels) * (screen_bottom_coord - screen_top_coord);

	if (view_type == PERSPECTIVE) {
		Vec3 ray_direction = Vec3(u, v, -focal_length);
		return Ray(origin, ray_direction);
	}
	else if (view_type == ORTHOGRAPHIC) {
		Vec3 ray_origin = Vec3(u, v, 0) + origin;
		return Ray(ray_origin, Vec3(0, 0, -1));
	}

	// Catch-all for invalid view types
	return Ray(origin, direction);
}

CameraBuilder::CameraBuilder() : camera(Camera()) {};

CameraBuilder* CameraBuilder::create() {
	return new CameraBuilder();
}

CameraBuilder* CameraBuilder::with_screen_dimensions(int width, int height) {
	camera.screen_width_pixels = width;
	camera.screen_height_pixels = height;
	return this;
}

CameraBuilder* CameraBuilder::with_focal_length(double focal_length) {
	camera.focal_length = focal_length;
	return this;
}

CameraBuilder* CameraBuilder::with_origin(Vec3 origin) {
	camera.origin = origin;
	return this;
}

CameraBuilder* CameraBuilder::with_direction(Vec3 direction) {
	camera.direction = direction;
	return this;
}

CameraBuilder* CameraBuilder::with_world_up(Vec3 direction) {
	camera.direction = direction;
	return this;
}

CameraBuilder* CameraBuilder::with_virtual_screen_boundaries(double left, double top, double right, double bottom) {
	camera.screen_left_coord = left;
	camera.screen_top_coord = top;
	camera.screen_right_coord = right;
	camera.screen_bottom_coord = bottom;
	return this;
}

CameraBuilder* CameraBuilder::with_view_type(ViewType view_type) {
	camera.view_type = view_type;
	return this;
}

CameraBuilder* CameraBuilder::with_color_channels(int color_channels) {
	camera.color_channels = color_channels;
	return this;
}

CameraBuilder* CameraBuilder::with_rays_per_pixel(int rays_per_pixel) {
	camera.rays_per_pixel = rays_per_pixel;
	return this;
}

CameraBuilder* CameraBuilder::with_max_recursion_depth(int max_recursion_depth) {
	camera.max_recursion_depth = max_recursion_depth;
	return this;
}

Camera CameraBuilder::build() const {
	if (camera.screen_width_pixels <= 0) {
		throw std::invalid_argument("Invalid camera screen width. Make sure to set it to a value greater than 0.");
	}
	if (camera.screen_height_pixels <= 0) {
		throw std::invalid_argument("Invalid camera screen height. Make sure to set it to a value greater than 0.");
	}
	if (camera.rays_per_pixel <= 0) {
		throw std::invalid_argument("Invalid number of rays per pixel. Make sure to set it to a value greater than 0.");
	}
	if (camera.max_recursion_depth <= 0) {
		throw std::invalid_argument("Invalid maximum recursion depth. Make sure to set it to a value greater than 0.");
	}
	return camera;
}

