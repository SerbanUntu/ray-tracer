#include <string>
#include "image.h"
#include "util/complex.h"

struct Point {
	double x;
	double y;
};

constexpr int WIDTH = 4000;
constexpr double ASPECT_RATIO = 16. / 9.;
constexpr double ZOOM = 100;
constexpr int MAX_ITERATIONS = 1000;
constexpr Point CENTER = Point(0.16125, 0.638438);
constexpr auto ESCAPE_BOUNDARY_SQUARED = 1 << 8; // Higher = better smoothing
const std::string FILENAME = "mandelbrot.bmp";

const double LOG_2 = std::log(2);
constexpr int HEIGHT = WIDTH / ASPECT_RATIO;
constexpr double LEFT = -(double)ASPECT_RATIO / ZOOM + CENTER.x;
const double RIGHT = (double)ASPECT_RATIO / ZOOM + CENTER.x;
const double BOTTOM = -1. / ZOOM + CENTER.y;
const double TOP = 1 / ZOOM + CENTER.y;

constexpr auto PALLETE_SIZE = 16;
const Vec3 PALLETE[PALLETE_SIZE] = {
	Vec3(0.094118, 0.321569, 0.694118), Vec3(0.223529, 0.490196, 0.819608), Vec3(0.525490, 0.709804, 0.898039), Vec3(0.827451, 0.925490, 0.972549),
	Vec3(0.945098, 0.913725, 0.749020), Vec3(0.972549, 0.788235, 0.372549), Vec3(1.000000, 0.666667, 0.000000), Vec3(0.800000, 0.501961, 0.000000),
	Vec3(0.600000, 0.341176, 0.000000), Vec3(0.415686, 0.203922, 0.011765), Vec3(0.258824, 0.117647, 0.058824), Vec3(0.098039, 0.027451, 0.101961),
	Vec3(0.035294, 0.003922, 0.184314), Vec3(0.015686, 0.015686, 0.286275), Vec3(0.000000, 0.027451, 0.392157), Vec3(0.047059, 0.172549, 0.541176)
};

double calculate_iterations(Complex c) {
	double i = 0;
	Complex z = Complex::ZERO;
	while (z.magnitude_squared() < ESCAPE_BOUNDARY_SQUARED && i < MAX_ITERATIONS) {
		z = z.squared() + c;
		i++;
	}

	if (i >= MAX_ITERATIONS) return i;

	// Smoothing
	double log_zn = std::log(z.magnitude_squared()) / 2.;
	double nu = std::log(log_zn / LOG_2) / LOG_2;
	return i + 1 - nu;
}

Vec3 get_mandelbrot_color(double iterations) {
	if (iterations >= MAX_ITERATIONS) return Vec3::ZERO;

	int integer = static_cast<int>(std::floor(iterations));
	double decimal = iterations - (double)integer;
	return PALLETE[integer % PALLETE_SIZE] +
		(PALLETE[(integer + 1) % PALLETE_SIZE] - PALLETE[integer % PALLETE_SIZE]) * decimal;
}

Complex get_coordinate(int row, int col) {
	double re = LEFT + ((double)col / ((double)WIDTH - 1.)) * (RIGHT - LEFT);
	double im = TOP + ((double)row / ((double)HEIGHT - 1.)) * (BOTTOM - TOP);
	return Complex(re, im);
}

int main() {
	Image img = Image(WIDTH, HEIGHT, 256, false);

	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			int iterations = -1;
			img.draw(row, col, get_mandelbrot_color(calculate_iterations(get_coordinate(row, col))));
		}
	}

	img.generateBmp(FILENAME);
}