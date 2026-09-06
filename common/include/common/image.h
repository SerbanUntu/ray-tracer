#pragma once
#include <vector>
#include "util/vec3.h"

class Image
{
    std::vector<Vec3> data;
    const int width, height;
    const int color_channels;
    const bool is_grayscale;

    void validate_dimensions(int x, int y) const;

public:
    Image(int _width, int _height, int _channels, bool _is_grayscale);
    explicit Image(const Vec3& color);
    void draw(int x, int y, const Vec3& color);
    void add_color(int x, int y, const Vec3& color);
    void generateBmp(const std::string& file_name) const;
    [[nodiscard]] int get_width() const { return width; }
    [[nodiscard]] int get_height() const { return height; }
    [[nodiscard]] int get_color_channels() const { return color_channels; }
    [[nodiscard]] bool get_is_grayscale() const { return is_grayscale; }
    [[nodiscard]] const std::vector<Vec3>& get_data() const { return data; }
    [[nodiscard]] Vec3 get_color(const int x, const int y) const { return data[x * width + y]; }
};
