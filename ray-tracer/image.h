#pragma once
#include <vector>
#include <stdexcept>
#include "util/vec3.h"


class Image {
private:
	std::vector<Vec3> data;
	const int width;
	const int height;
	const int color_channels;
	const bool is_grayscale;

	void validate_dimensions(int x, int y) const {
		if (x >= height) throw std::invalid_argument("Height exceeded");
		if (y >= width) throw std::invalid_argument("Width exceeded");
	}

public:
	Image(int _width, int _height, int _channels, bool _is_grayscale) :
		width(_width), height(_height), color_channels(_channels), is_grayscale(_is_grayscale) {
		for (int i = 0; i < _width * _height; i++) {
			data.push_back(Vec3::ZERO);
		}
	}
	void draw(int x, int y, Vec3 color) {
		validate_dimensions(x, y);
		data[x * width + y] = color;
	}
	void add_color(int x, int y, Vec3 color) {
		validate_dimensions(x, y);
		data[x * width + y] += color;
	}
	void generateBmp(std::string file_name);
};