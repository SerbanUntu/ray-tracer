#pragma once
#include <vector>
#include <stdexcept>
#include "util/vec3.h"

static std::byte doubleToColorByte(double value, int channels);
static void write_n_bytes(std::vector<std::byte>& vec, int n, int data);

class Image {
private:
	std::vector<Vec3> data;
	const int width, height;
	const int color_channels;
	const bool is_grayscale;

	void validate_dimensions(int x, int y) const;

public:
	Image(int _width, int _height, int _channels, bool _is_grayscale);
	Image(Vec3 color);
	void draw(int x, int y, Vec3 color);
	void add_color(int x, int y, Vec3 color);
	int get_width() const;
	int get_height() const;
	int get_color_channels() const;
	bool get_is_grayscale() const;
	const std::vector<Vec3>& get_data() const;
	Vec3 get_color(int x, int y) const;
	void generateBmp(std::string file_name);
};