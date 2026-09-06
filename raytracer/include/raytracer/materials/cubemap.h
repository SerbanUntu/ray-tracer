#pragma once
#include <cstddef>
#include "common/image.h"

namespace raytracer::raytracer
{
enum class TextureExtension
{
    REPEAT,
    CLAMP,
    CONSTANT
};

void draw_face(int i, int j, int pos, const std::vector<std::byte>& buffer, common::Image* img);

class Cubemap
{
    common::Image top, left, front, right, back, bottom;

public:
    Cubemap(const std::string& path, int face_width, int face_height, int color_channels, bool is_grayscale);
    Cubemap(common::Image _top, common::Image _left, common::Image _front, common::Image _right, common::Image _back, common::Image _bottom);
    [[nodiscard]] common::Vec3 get_color_at_point(const common::Vec3& normal) const;
};
} // namespace raytracer::raytracer
