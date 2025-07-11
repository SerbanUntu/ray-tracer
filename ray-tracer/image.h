#pragma once
#include <array>
#include <stdexcept>
#include "color.h"

class Image {
private:
	std::array<Color, 200 * 200> data;
	const int width;
	const int height;

public:
	Image(int _width, int _height) : width(_width), height(_height) {
	}
	inline void draw(int x, int y, Color color) {
		if (x >= height) throw std::invalid_argument("Height exceeded");
		if (y >= width) throw std::invalid_argument("Width exceeded");
		data[x * width + y] = color;
	}
	void generateBmp(std::string file_name);
};