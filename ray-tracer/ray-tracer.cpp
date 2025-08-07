#include "ray-tracer.h"
#include <vector>
#include <array>
#include <fstream>
#include "color.h"
#include "image.h"
#include "sphere.h"
#include "util/vec3.h"

const int LEFT = -1;
const int RIGHT = 1;
const int BOTTOM = -1;
const int TOP = 1;
const double FOCAL_LENGTH = 1;
const Vec3 ORIGIN = Vec3(0, 0, 0);
const int NO_PIXELS = 800;

static void write_n_bytes(std::vector<std::byte>& vec, int n, int data) {
	// Increasing offset due to little-endian ordering (LSB first)
	int offset = 0;
	for (int i = 0; i < n; i++) {
		vec.push_back(std::byte((data >> offset) & 255));
		offset += 8;
	}
}

void Image::generateBmp(std::string file_name) {
	// Writing the header
	std::vector<std::byte> image_bytes;

	// File Header signature
	image_bytes.push_back(std::byte('B'));
	image_bytes.push_back(std::byte('M'));

	const int file_size = 14 + 40 + 3 * width * height;
	write_n_bytes(image_bytes, 4, file_size);

	const int reserved_field = 0;
	write_n_bytes(image_bytes, 4, reserved_field);

	const int pixel_data_offset = 14 + 40;
	write_n_bytes(image_bytes, 4, pixel_data_offset);

	const int bitmap_header_size = 40;
	write_n_bytes(image_bytes, 4, bitmap_header_size);

	write_n_bytes(image_bytes, 4, width);
	write_n_bytes(image_bytes, 4, height);

	const int bmp_reserved_field = 1;
	write_n_bytes(image_bytes, 2, bmp_reserved_field);

	const int bits_per_pixel = 24;
	write_n_bytes(image_bytes, 2, bits_per_pixel);

	const int compression = 0;
	write_n_bytes(image_bytes, 4, compression);

	const int size_of_pixel_data = 3 * width * height;
	write_n_bytes(image_bytes, 4, size_of_pixel_data);

	const int horizontal_resolution = 2835;
	write_n_bytes(image_bytes, 4, horizontal_resolution);

	const int vertical_resolution = 2835;
	write_n_bytes(image_bytes, 4, vertical_resolution);

	const int color_palette_info = 0;
	write_n_bytes(image_bytes, 4, color_palette_info);

	const int no_important_colors = 0;
	write_n_bytes(image_bytes, 4, no_important_colors);

	for (int i = 0; i < width * height; i++) {
		image_bytes.push_back(std::byte(data[i].get_blue()));
		image_bytes.push_back(std::byte(data[i].get_green()));
		image_bytes.push_back(std::byte(data[i].get_red()));
	}

	std::ofstream image_file(file_name);

	image_file.write(reinterpret_cast<const char*>(image_bytes.data()), image_bytes.size());
	image_file.close();
}

const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);

int main()
{
	Image img = Image(NO_PIXELS, NO_PIXELS);
	const std::array<Sphere, 3> objects = {
		Sphere(Vec3(0, 4, -10), 4, Color::RED),
		Sphere(Vec3(0, 0, -20), 4, Color::GREEN),
		Sphere(Vec3(0, -4, -30), 4, Color::BLUE),
	};

	for (int i = 0; i < NO_PIXELS; i++) {
		for (int j = 0; j < NO_PIXELS; j++) {
			const double u = LEFT + ((double)j / (double)NO_PIXELS) * (RIGHT - LEFT);
			const double v = TOP + ((double)i / (double)NO_PIXELS) * (BOTTOM - TOP);
			Vec3 ray_direction = Vec3(u, v, -FOCAL_LENGTH);

			double min_depth = DBL_MAX;
			Color top_color = Color(0, 0, 0); // Background color

			for (const Sphere s : objects) {
				const double t = s.ray_intersection(Ray(ORIGIN, ray_direction));
				if (t < min_depth && t > 0) {
					min_depth = t;
					top_color = s.get_color();
				}
			}

			img.draw(i, j, top_color);
		}
	}
	img.generateBmp("my_file.bmp");
	return 0;
}
