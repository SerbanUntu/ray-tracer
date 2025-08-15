#pragma once
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "image.h"
#include "sphere.h"
#include "util/vec3.h"

enum TextureExtension {
	REPEAT,
	CLAMP,
	CONSTANT
};

void draw_face(int i, int j, int pos, std::vector<char> buffer, Image* img) {

	double b = static_cast<int>(static_cast<unsigned char>(buffer[pos]));
	double g = static_cast<int>(static_cast<unsigned char>(buffer[pos + 1]));
	double r = static_cast<int>(static_cast<unsigned char>(buffer[pos + 2]));

	Vec3 col = Vec3(r / 256., g / 256., b / 256.);

	img->draw(i, j, col);
}

class Cubemap {
private:
	Image top;
	Image left;
	Image front;
	Image right;
	Image back;
	Image bottom;

public:
	Cubemap(std::string path, int face_width, int face_height, int color_channels, bool is_grayscale) :
		top(Image(face_width, face_height, color_channels, is_grayscale)),
		left(Image(face_width, face_height, color_channels, is_grayscale)),
		front(Image(face_width, face_height, color_channels, is_grayscale)),
		right(Image(face_width, face_height, color_channels, is_grayscale)),
		back(Image(face_width, face_height, color_channels, is_grayscale)),
		bottom(Image(face_width, face_height, color_channels, is_grayscale)) {

		std::vector<char> buffer;

		std::ifstream file(path, std::ios::binary);

		if (!file) {
			throw std::runtime_error("Cannot open file");
		}

		file.seekg(0, std::ios::end);
		if (file.fail()) {
			throw std::runtime_error("seekg failed");
		}

		std::streampos pos = file.tellg();
		if (pos == std::streampos(-1)) {
			throw std::runtime_error("tellg failed");
		}

		size_t length = static_cast<size_t>(pos);

		buffer.resize(length);

		file.seekg(0, std::ios::beg);
		file.read(&buffer[0], length);

		const int HEADER_LENGTH = 54;

		// TOP
		for (int i = 0; i < face_height; i++) {
			for (int j = 0; j < face_width; j++) {
				int actual_i = 3 * face_height - i - 1;
				int actual_j = face_width + j;
				draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &top);
			}
		}

		// LEFT
		for (int i = 0; i < face_height; i++) {
			for (int j = 0; j < face_width; j++) {
				int actual_i = 2 * face_height - i - 1;
				int actual_j = j;
				draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &left);
			}
		}

		// FRONT
		for (int i = 0; i < face_height; i++) {
			for (int j = 0; j < face_width; j++) {
				int actual_i = 2 * face_height - i - 1;
				int actual_j = j + face_width;
				draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &front);
			}
		}

		// RIGHT
		for (int i = 0; i < face_height; i++) {
			for (int j = 0; j < face_width; j++) {
				int actual_i = 2 * face_height - i - 1;
				int actual_j = j + 2 * face_width;
				draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &right);
			}
		}

		// BACK
		for (int i = 0; i < face_height; i++) {
			for (int j = 0; j < face_width; j++) {
				int actual_i = 2 * face_height - i - 1;
				int actual_j = j + 3 * face_width;
				draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &back);
			}
		}

		// BOTTOM
		for (int i = 0; i < face_height; i++) {
			for (int j = 0; j < face_width; j++) {
				int actual_i = face_height - i - 1;
				int actual_j = j + face_width;
				draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &bottom);
			}
		}

		file.close();
	}

	Cubemap(Image _top, Image _left, Image _front, Image _right, Image _back, Image _bottom) :
		top(_top), left(_left), front(_front), right(_right), back(_back), bottom(_bottom) {
	}

	/**
	 Calculates the texture color at a point on the sphere, given the normal.

	 @param normal The normal to the sphere surface at that point
	 @return A vector representing the color at that point
	 */
	Vec3 get_color_at_point(Vec3 normal) const {
		Vec3 direction = normal.to_normalized();
		double nx = std::abs(direction.x);
		double ny = std::abs(direction.y);
		double nz = std::abs(direction.z);

		// TOP
		if (ny >= nz && ny >= nx && direction.y >= 0) {
			double normalized_x = (direction.x + 1) / 2 * top.get_width();
			double normalized_z = (direction.z + 1) / 2 * top.get_height();
			int x = std::floor(normalized_x);
			int z = std::floor(normalized_z);
			return top.get_color(x, z);
		}

		// BOTTOM
		else if (ny >= nz && ny >= nx && direction.y < 0) {
			double normalized_x = (direction.x + 1) / 2 * bottom.get_width();
			double normalized_z = (direction.z + 1) / 2 * bottom.get_height();
			int x = std::floor(normalized_x);
			int z = std::floor(normalized_z);
			return bottom.get_color(x, z);
		}

		// LEFT
		else if (nx >= ny && nx >= nz && direction.x < 0) {
			double normalized_y = (direction.y + 1) / 2 * left.get_width();
			double normalized_z = (direction.z + 1) / 2 * left.get_height();
			int y = std::floor(normalized_y);
			int z = std::floor(normalized_z);
			return left.get_color(y, z);
		}

		// RIGHT
		else if (nx >= ny && nx >= nz && direction.x >= 0) {
			double normalized_y = (direction.y + 1) / 2 * right.get_width();
			double normalized_z = (direction.z + 1) / 2 * right.get_height();
			int y = std::floor(normalized_y);
			int z = std::floor(normalized_z);
			return right.get_color(y, z);
		}

		// FRONT
		else if (nz >= nx && nz >= ny && direction.z >= 0) {
			double normalized_x = (direction.x + 1) / 2 * front.get_width();
			double normalized_y = (direction.y + 1) / 2 * front.get_height();
			int x = std::floor(normalized_x);
			int y = std::floor(normalized_y);
			return front.get_color(x, y);
		}

		// BACK
		else {
			double normalized_x = (direction.x + 1) / 2 * back.get_width();
			double normalized_y = (direction.y + 1) / 2 * back.get_height();
			int x = std::floor(normalized_x);
			int y = std::floor(normalized_y);
			return back.get_color(x, y);
		}
	}
};
