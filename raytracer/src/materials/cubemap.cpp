#include "raytracer/materials/cubemap.h"
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace raytracer::raytracer
{
void draw_face(const int i, const int j, const int pos, const std::vector<std::byte>& buffer, common::Image* img)
{
    const double b = std::to_integer<int>(buffer[pos]);
    const double g = std::to_integer<int>(buffer[pos + 1]);
    const double r = std::to_integer<int>(buffer[pos + 2]);

    const auto col = common::Vec3(r / 255., g / 255., b / 255.);

    img->draw(i, j, col);
}

Cubemap::Cubemap(const std::string& path, int face_width, int face_height, int color_channels, bool is_grayscale) :
    top(common::Image(face_width, face_height, color_channels, is_grayscale)),
    left(common::Image(face_width, face_height, color_channels, is_grayscale)),
    front(common::Image(face_width, face_height, color_channels, is_grayscale)),
    right(common::Image(face_width, face_height, color_channels, is_grayscale)),
    back(common::Image(face_width, face_height, color_channels, is_grayscale)),
    bottom(common::Image(face_width, face_height, color_channels, is_grayscale))
{
    std::vector<std::byte> buffer;

    std::ifstream file(path, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Cannot open file " + path);
    }

    file.seekg(0, std::ios::end);
    if (file.fail())
    {
        throw std::runtime_error("File size measurement failed during seekg.");
    }

    std::streampos pos = file.tellg();
    if (pos == std::streampos(-1))
    {
        throw std::runtime_error("File size measurement failed during tellg.");
    }

    const size_t length = pos;

    constexpr int HEADER_LENGTH = 54;

    const size_t required_length = HEADER_LENGTH + static_cast<size_t>(face_width) * 4 * face_height * 3 * 3;
    if (length < required_length)
    {
        throw std::runtime_error("Cubemap file " + path + " is too small for the requested face dimensions.");
    }

    buffer.resize(length);

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer.data()), static_cast<long long>(length));

    // TOP
    for (int i = 0; i < face_height; i++)
    {
        for (int j = 0; j < face_width; j++)
        {
            int actual_i = 3 * face_height - i - 1;
            int actual_j = face_width + j;
            draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &top);
        }
    }

    // LEFT
    for (int i = 0; i < face_height; i++)
    {
        for (int j = 0; j < face_width; j++)
        {
            int actual_i = 2 * face_height - i - 1;
            int actual_j = j;
            draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &left);
        }
    }

    // FRONT
    for (int i = 0; i < face_height; i++)
    {
        for (int j = 0; j < face_width; j++)
        {
            int actual_i = 2 * face_height - i - 1;
            int actual_j = j + face_width;
            draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &front);
        }
    }

    // RIGHT
    for (int i = 0; i < face_height; i++)
    {
        for (int j = 0; j < face_width; j++)
        {
            int actual_i = 2 * face_height - i - 1;
            int actual_j = j + 2 * face_width;
            draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &right);
        }
    }

    // BACK
    for (int i = 0; i < face_height; i++)
    {
        for (int j = 0; j < face_width; j++)
        {
            int actual_i = 2 * face_height - i - 1;
            int actual_j = j + 3 * face_width;
            draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &back);
        }
    }

    // BOTTOM
    for (int i = 0; i < face_height; i++)
    {
        for (int j = 0; j < face_width; j++)
        {
            int actual_i = face_height - i - 1;
            int actual_j = j + face_width;
            draw_face(i, j, (actual_i * face_width * 4 + actual_j) * 3 + HEADER_LENGTH, buffer, &bottom);
        }
    }

    file.close();
}

Cubemap::Cubemap(common::Image _top, common::Image _left, common::Image _front, common::Image _right,
                 common::Image _back, common::Image _bottom) :
    top(std::move(_top)), left(std::move(_left)), front(std::move(_front)), right(std::move(_right)),
    back(std::move(_back)), bottom(std::move(_bottom))
{
}

/**
 Calculates the texture color at a point on the sphere, given the normal.

 @param normal The normal to the sphere surface at that point
 @return A vector representing the color at that point
 */
common::Vec3 Cubemap::get_color_at_point(const common::Vec3& normal) const
{
    const common::Vec3 direction = normal.to_normalized();
    const double nx = std::abs(direction.x);
    const double ny = std::abs(direction.y);
    const double nz = std::abs(direction.z);

    const auto face_col = [](const double component, const common::Image& face) -> int
    {
        return std::clamp(static_cast<int>(std::floor((component + 1) / 2 * face.get_width())), 0,
                          face.get_width() - 1);
    };
    const auto face_row = [](const double component, const common::Image& face) -> int
    {
        return std::clamp(static_cast<int>(std::floor((component + 1) / 2 * face.get_height())), 0,
                          face.get_height() - 1);
    };

    // TOP
    if (ny >= nz && ny >= nx && direction.y >= 0)
    {
        return top.get_color(face_row(direction.z, top), face_col(direction.x, top));
    }

    // BOTTOM
    if (ny >= nz && ny >= nx && direction.y < 0)
    {
        return bottom.get_color(face_row(direction.z, bottom), face_col(direction.x, bottom));
    }

    // LEFT
    if (nx >= ny && nx >= nz && direction.x < 0)
    {
        return left.get_color(face_row(direction.z, left), face_col(direction.y, left));
    }

    // RIGHT
    if (nx >= ny && nx >= nz && direction.x >= 0)
    {
        return right.get_color(face_row(direction.z, right), face_col(direction.y, right));
    }

    // FRONT
    if (nz >= nx && nz >= ny && direction.z >= 0)
    {
        return front.get_color(face_row(direction.y, front), face_col(direction.x, front));
    }

    // BACK
    {
        return back.get_color(face_row(direction.y, back), face_col(direction.x, back));
    }
}
} // namespace raytracer::raytracer
