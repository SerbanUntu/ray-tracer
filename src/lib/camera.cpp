#include "camera.h"
#include <random>
#include <stdexcept>
#include "util/random_utils.h"

static std::uniform_real_distribution offset_dist(-.5, .5);

Camera::Camera(
	const double _screen_left_coord,
	const double _screen_right_coord,
	const double _screen_bottom_coord,
	const double _screen_top_coord,
	const double _focal_length,
	const Vec3& _origin,
	const Vec3& _direction,
	const Vec3& _world_up,
	const int _screen_width_pixels,
	const int _screen_height_pixels,
	const ViewType _view_type,
	const int _color_channels,
	const int _rays_per_pixel,
	const int _max_recursion_depth
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

Ray Camera::compute_ray_for_pixel(const Pixel p) const {

	const Vec3 forward = direction.to_normalized();
	const Vec3 right = forward.cross(world_up).to_normalized();
	const Vec3 up = right.cross(forward);

	const double aspect_ratio = static_cast<double>(screen_width_pixels) / screen_height_pixels;
	const double CAMERA_LEFT = screen_left_coord * aspect_ratio;
	const double CAMERA_RIGHT = screen_right_coord * aspect_ratio;

	const double RANDOM_X = static_cast<double>(p.x) + .5 + (rays_per_pixel > 1 ? offset_dist(get_generator()) : 0);
	const double RANDOM_Y = static_cast<double>(p.y) + .5 + (rays_per_pixel > 1 ? offset_dist(get_generator()) : 0);

	const double u = CAMERA_LEFT + RANDOM_X / static_cast<double>(screen_width_pixels) * (CAMERA_RIGHT - CAMERA_LEFT);
	const double v = screen_top_coord + RANDOM_Y / static_cast<double>(screen_height_pixels) * (screen_bottom_coord - screen_top_coord);

	if (view_type == ViewType::PERSPECTIVE) {
		const Vec3 ray_direction = right * u + up * v + forward * focal_length;
		return Ray(origin, ray_direction);
	}
	if (view_type == ViewType::ORTHOGRAPHIC) {
		const Vec3 ray_origin = right * u + up * v + origin;
		return Ray(ray_origin, forward);
	}

	// Catch-all for invalid view types
	return Ray(origin, forward);
}

CameraBuilder::CameraBuilder() : camera(Camera()) {};

CameraBuilder& CameraBuilder::with_screen_dimensions(const int width, const int height) & {
	camera.screen_width_pixels = width;
	camera.screen_height_pixels = height;
	return *this;
}

CameraBuilder& CameraBuilder::with_focal_length(const double focal_length) & {
	camera.focal_length = focal_length;
	return *this;
}

CameraBuilder& CameraBuilder::with_origin(Vec3 origin) & {
	camera.origin = origin;
	return *this;
}

CameraBuilder& CameraBuilder::with_direction(Vec3 direction) & {
	camera.direction = direction;
	return *this;
}

CameraBuilder& CameraBuilder::with_world_up(Vec3 direction) & {
	camera.world_up = direction;
	return *this;
}

CameraBuilder& CameraBuilder::with_virtual_screen_boundaries(const double left, const double top, const double right, const double bottom) & {
	camera.screen_left_coord = left;
	camera.screen_top_coord = top;
	camera.screen_right_coord = right;
	camera.screen_bottom_coord = bottom;
	return *this;
}

CameraBuilder& CameraBuilder::with_view_type(const ViewType view_type) & {
	camera.view_type = view_type;
	return *this;
}

CameraBuilder& CameraBuilder::with_color_channels(const int color_channels) & {
	camera.color_channels = color_channels;
	return *this;
}

CameraBuilder& CameraBuilder::with_rays_per_pixel(const int rays_per_pixel) & {
	camera.rays_per_pixel = rays_per_pixel;
	return *this;
}

CameraBuilder& CameraBuilder::with_max_recursion_depth(const int max_recursion_depth) & {
	camera.max_recursion_depth = max_recursion_depth;
	return *this;
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
	if (camera.direction.cross(camera.world_up).length() == 0) {
		throw std::invalid_argument("Invalid camera orientation. Make sure the direction and world up vectors are non-zero and not parallel.");
	}
	return camera;
}

