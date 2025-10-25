#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "image.h"
#include "util/complex.h"
#include "scene.h"

using json = nlohmann::json;
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MandelbrotSceneConfig, center, width, aspect_ratio, zoom, max_iterations, escape_boundary_squared, output_path)

constexpr auto PALLETE_SIZE = 16;
const Vec3 PALLETE[PALLETE_SIZE] = {
	Vec3(0.094118, 0.321569, 0.694118), Vec3(0.223529, 0.490196, 0.819608), Vec3(0.525490, 0.709804, 0.898039), Vec3(0.827451, 0.925490, 0.972549),
	Vec3(0.945098, 0.913725, 0.749020), Vec3(0.972549, 0.788235, 0.372549), Vec3(1.000000, 0.666667, 0.000000), Vec3(0.800000, 0.501961, 0.000000),
	Vec3(0.600000, 0.341176, 0.000000), Vec3(0.415686, 0.203922, 0.011765), Vec3(0.258824, 0.117647, 0.058824), Vec3(0.098039, 0.027451, 0.101961),
	Vec3(0.035294, 0.003922, 0.184314), Vec3(0.015686, 0.015686, 0.286275), Vec3(0.000000, 0.027451, 0.392157), Vec3(0.047059, 0.172549, 0.541176)
};

double calculate_iterations(Complex c, int escape_boundary_squared, int max_iterations) {
	double i = 0;
	Complex z = Complex::ZERO;
	while (z.magnitude_squared() < escape_boundary_squared && i < max_iterations) {
		z = z.squared() + c;
		i++;
	}

	if (i >= max_iterations) return i;

	// Smoothing
	double log_zn = std::log(z.magnitude_squared()) / 2.;
	const double log_2 = std::log(2);
	double nu = std::log(log_zn / log_2) / log_2;
	return i + 1 - nu;
}

Vec3 get_mandelbrot_color(double iterations, int max_iterations) {
	if (iterations >= max_iterations) return Vec3::ZERO;

	int integer = static_cast<int>(std::floor(iterations));
	double decimal = iterations - (double)integer;
	return PALLETE[integer % PALLETE_SIZE] +
		(PALLETE[(integer + 1) % PALLETE_SIZE] - PALLETE[integer % PALLETE_SIZE]) * decimal;
}

Complex get_coordinate(int row, int col, MandelbrotSceneSpace mss) {
	double re = mss.left + ((double)col / ((double)mss.width_pixels - 1.)) * (mss.right - mss.left);
	double im = mss.top + ((double)row / ((double)mss.height_pixels - 1.)) * (mss.bottom - mss.top);
	return Complex(re, im);
}

int main() {
	std::ifstream i(MANDELBROT_DATA_PATH);

	json j;
	i >> j;
	MandelbrotSceneConfig scene;

	try {
		scene = j;
	} catch (const std::exception& e) {
		std::cerr << "JSON parsing failed: " << e.what() << '\n';
		return -1;
	}

	const int HEIGHT = scene.width / scene.aspect_ratio;
	const double LEFT = -(double)scene.aspect_ratio / scene.zoom + scene.center.x;
	const double RIGHT = (double)scene.aspect_ratio / scene.zoom + scene.center.x;
	const double BOTTOM = -1. / scene.zoom + scene.center.y;
	const double TOP = 1 / scene.zoom + scene.center.y;

	Image img = Image(scene.width, HEIGHT, 256, false);
	const MandelbrotSceneSpace mss(
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,
		HEIGHT,
		scene.width,
		scene.center
	);

	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < scene.width; col++) {
			int iterations = -1;
			img.draw(row, col, 
				get_mandelbrot_color(
					calculate_iterations(
						get_coordinate(row, col, mss),
						scene.escape_boundary_squared,
						scene.max_iterations
					),
					scene.max_iterations
				)
			);
		}
	}

	img.generateBmp(scene.output_path);
	return 0;
}