#include <string>
#include <image.h>
#include <util/complex.h>

struct Point {
	double x;
	double y;
};

constexpr int WIDTH = 1000;
constexpr double ASPECT_RATIO = 16. / 9.;
constexpr double ZOOM = 0.8;
constexpr int MAX_ITERATIONS = 100;
constexpr Point CENTER = Point(-0.5, 0);
const std::string FILENAME = "mandelbrot.bmp";

constexpr int HEIGHT = WIDTH / ASPECT_RATIO;
constexpr double LEFT = -(double)ASPECT_RATIO / ZOOM + CENTER.x;
const double RIGHT = (double)ASPECT_RATIO / ZOOM + CENTER.x;
const double BOTTOM = -1. / ZOOM + CENTER.y;
const double TOP = 1 / ZOOM + CENTER.y;

constexpr auto PALLETE_SIZE = 4;
const Vec3 PALLETE[PALLETE_SIZE] = {
	Vec3::ZERO,      // Black
	Vec3(0, 0, 1),   // Blue
	Vec3(1, 1, 0),   // Yellow
	Vec3::ZERO,      // Black
};

int calculate_iterations(Complex c) {
	int i = 0;
	Complex z = Complex::ZERO;
	while (z.magnitude_squared() < 4 && i < MAX_ITERATIONS) {
		z = z.squared() + c;
		i++;
	}
	return i;
}

Vec3 get_mandelbrot_color(int iterations) {

	// Each number of iterations is between two well-defined bands of color;
	// The lower band and the upper band

	// The index of the lower color band for the given number of iterations
	const int intensity = static_cast<int>(
		std::floor((double)iterations * ((double)PALLETE_SIZE - 1) / MAX_ITERATIONS));

	// The minimum number of iterations that would lead to the same lower band as the input
	const int boundary = intensity * MAX_ITERATIONS / (PALLETE_SIZE - 1);

	// The factor by which the number of iterations is closer to the
	// upper band compared to the lower band (0 means at lower, 1 means at upper)
	const double normalized_progress =
		(double)(iterations - boundary) / ((double)MAX_ITERATIONS / (PALLETE_SIZE - 1));

	return PALLETE[intensity] + (PALLETE[intensity + 1] - PALLETE[intensity]) * normalized_progress;
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