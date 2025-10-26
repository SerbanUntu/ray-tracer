#include <gtest/gtest.h>
#include "scene.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(MandelbrotJSONTest, PointParsing) {
	json j = {
		{"x", 1.},
		{"y", 2.},
	};
	Point p = j;
	EXPECT_EQ(1., p.x);
	EXPECT_EQ(2., p.y);
}

TEST(MandelbrotJSONTest, PointSerializing) {
	Point p = Point(1., 2.);
	json j = p;
	EXPECT_EQ(1., j["x"]);
	EXPECT_EQ(2., j["y"]);
}

TEST(MandelbrotJSONTest, SceneConfigParsing) {
	json j = {
		{
			"center", {
				{"x", 1.},
				{"y", 2.},
			},
		},
		{"width", 1920},
		{"aspect_ratio", 1.6},
		{"zoom", 100.},
		{"max_iterations", 50},
		{"escape_boundary_squared", 256},
		{"output_path", "mandelbrot.bmp"},
	};
	MandelbrotSceneConfig msc = j;
	EXPECT_EQ(1., msc.center.x);
	EXPECT_EQ(2., msc.center.y);
	EXPECT_EQ(1920, msc.width);
	EXPECT_EQ(1.6, msc.aspect_ratio);
	EXPECT_EQ(100., msc.zoom);
	EXPECT_EQ(50, msc.max_iterations);
	EXPECT_EQ(256, msc.escape_boundary_squared);
	EXPECT_EQ("mandelbrot.bmp", msc.output_path);
}

TEST(MandelbrotJSONTest, SceneConfigSerializing) {
	MandelbrotSceneConfig msc = MandelbrotSceneConfig(
		Point(1., 2.),
		1920,
		1.6,
		100.,
		50,
		256,
		"mandelbrot.bmp"
	);
	json j = msc;
	EXPECT_EQ(1., j["center"]["x"]);
	EXPECT_EQ(2., j["center"]["y"]);
	EXPECT_EQ(1920, j["width"]);
	EXPECT_EQ(1.6, j["aspect_ratio"]);
	EXPECT_EQ(100., j["zoom"]);
	EXPECT_EQ(50, j["max_iterations"]);
	EXPECT_EQ(256, j["escape_boundary_squared"]);
	EXPECT_EQ("mandelbrot.bmp", j["output_path"]);
}

