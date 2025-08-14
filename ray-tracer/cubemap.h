#pragma once
#include "image.h"
#include "sphere.h"
#include "math.h"
#include "util/vec3.h"

enum TextureExtension {
	REPEAT,
	CLAMP,
	CONSTANT
};

class Cubemap {
public:
	Image top;
	Image left;
	Image front;
	Image right;
	Image back;
	Image bottom;

	Cubemap(Image _top, Image _left, Image _front, Image _right, Image _back, Image _bottom) :
		top(_top), left(_left), front(_front), right(_right), back(_back), bottom(_bottom) {
	}

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
