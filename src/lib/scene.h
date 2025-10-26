#pragma once
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "util/vec3.h"
#include "camera.h"
#include "shapes/object.h"
#include "materials/cubemap.h"
#include "materials/material.h"

using json = nlohmann::json;

struct RayTracerSceneConfig {
	Vec3 background_color;
	Vec3 sky_color;
	Camera camera;

	std::vector<std::unique_ptr<Object>> objects;

	// Post-processing
	bool is_shading_thresholded;
	double thresholded_shading_degrees;
	bool is_grayscale;

	std::string output_path;
};

struct Point {
	double x;
	double y;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y);

struct MandelbrotSceneConfig {
	Point center;
	int width;
	double aspect_ratio;
	double zoom;
	int max_iterations;
	int escape_boundary_squared;
	std::string output_path;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	MandelbrotSceneConfig,
	center,
	width,
	aspect_ratio,
	zoom,
	max_iterations,
	escape_boundary_squared,
	output_path
);

struct MandelbrotSceneSpace {
	double left;
	double right;
	double bottom;
	double top;
	int height_pixels;
	int width_pixels;
	Point center;
};