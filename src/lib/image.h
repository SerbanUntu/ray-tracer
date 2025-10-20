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
	Image(Vec3 color) :
		width(1), height(1), color_channels(256), is_grayscale(false) {
		data.push_back(color);
	}
	void draw(int x, int y, Vec3 color) {
		validate_dimensions(x, y);
		data[x * width + y] = color;
	}
	void add_color(int x, int y, Vec3 color) {
		validate_dimensions(x, y);
		data[x * width + y] += color;
	}
	int get_width() const { return width; }
	int get_height() const { return height; }
	int get_color_channels() const { return color_channels; }
	bool get_is_grayscale() const { return is_grayscale; }
	const std::vector<Vec3>& get_data() const { return data; }
	Vec3 get_color(int x, int y) const {
		return data[x * width + y];
	}
	void generateBmp(std::string file_name);
};