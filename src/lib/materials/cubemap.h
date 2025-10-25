#pragma once
#include "image.h"

enum TextureExtension {
	REPEAT,
	CLAMP,
	CONSTANT
};

void draw_face(int i, int j, int pos, std::vector<char> buffer, Image* img);

class Cubemap {
private:
	Image top, left, front, right, back, bottom;

public:
	Cubemap(std::string path, int face_width, int face_height, int color_channels, bool is_grayscale);
	Cubemap(Image _top, Image _left, Image _front, Image _right, Image _back, Image _bottom);
	Vec3 get_color_at_point(Vec3 normal) const;
};
