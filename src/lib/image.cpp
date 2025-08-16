#include "image.h"
#include <algorithm>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstddef>

static std::byte doubleToColorByte(double value, int channels) {
	if (std::isnan(value) || value < 0.0) value = 0.0;
	if (value > 1.0) value = 1.0;
	double thresholded = static_cast<double>(static_cast<int>(
		value * channels))
		/ channels;
	int intVal = static_cast<int>(thresholded * 255.0 + 0.5);
	intVal = std::clamp(intVal, 0, 255);
	return static_cast<std::byte>(static_cast<uint8_t>(intVal));
}

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

	for (int r = height - 1; r >= 0; r--) {

		for (int c = 0; c < width; c++) {

			int i = r * width + c;

			if (is_grayscale) {
				const double gray = (data[i].x + data[i].y + data[i].z) / 3;
				const std::byte gray_byte = doubleToColorByte(gray, color_channels);
				image_bytes.push_back(gray_byte);
				image_bytes.push_back(gray_byte);
				image_bytes.push_back(gray_byte);
			}
			else {
				image_bytes.push_back(doubleToColorByte(data[i].z, color_channels));
				image_bytes.push_back(doubleToColorByte(data[i].y, color_channels));
				image_bytes.push_back(doubleToColorByte(data[i].x, color_channels));
			}

		}
		int bytes_so_far = width * 3;
		while (bytes_so_far % 4 != 0) {
			image_bytes.push_back(std::byte(0));
			bytes_so_far++;
		}
	}

	// std::ios::binary needed for binary data instead of text data on Windows
	std::ofstream image_file(file_name, std::ios::binary);

	image_file.write(reinterpret_cast<const char*>(image_bytes.data()), image_bytes.size());
	image_file.close();
}
