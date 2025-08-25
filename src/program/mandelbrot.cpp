#include <string>
#include <image.h>
#include <util/complex.h>

const std::string FILENAME = "mandelbrot.bmp";
const int WIDTH = 2000;
const int HEIGHT = 2000;
const double LEFT = -3;
const double RIGHT = 1;
const double BOTTOM = -2;
const double TOP = 2;
const int MAX_ITERATIONS = 256;

bool is_in_mandelbrot(Complex c, int* iterations) {
	int i = 0;
	Complex z = Complex::ZERO;
	while (i < MAX_ITERATIONS) {
		if (z.magnitude_squared() > 4) {
			*iterations = i;
			return false;
		}
		z = z.squared() + c;
		i++;
	}
	return true;
}

Vec3 get_mandelbrot_color(bool is_in_mandelbrot, int iterations) {
	if (is_in_mandelbrot) return Vec3::ZERO;
	else {
		const double intensity = (double)iterations / 256;
		return Vec3(intensity, intensity, intensity);
	}
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
			bool is_in = is_in_mandelbrot(get_coordinate(row, col), &iterations);
			img.draw(row, col, get_mandelbrot_color(is_in, iterations));
		}
	}

	img.generateBmp(FILENAME);
}