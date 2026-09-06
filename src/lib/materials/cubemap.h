#pragma once
#include "image.h"

enum class TextureExtension
{
    REPEAT,
    CLAMP,
    CONSTANT
};

void draw_face(int i, int j, int pos, const std::vector<char>& buffer, Image* img);

class Cubemap
{
    Image top, left, front, right, back, bottom;

public:
    Cubemap(const std::string& path, int face_width, int face_height, int color_channels, bool is_grayscale);
    Cubemap(Image _top, Image _left, Image _front, Image _right, Image _back, Image _bottom);
    [[nodiscard]] Vec3 get_color_at_point(const Vec3& normal) const;
};
