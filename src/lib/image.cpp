#include "image.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <cstddef>
#include <cstdint>

std::byte doubleToColorByte(const double value, const int channels)
{
    double normalizedValue = value;
    if (std::isnan(value) || value < 0.0) normalizedValue = 0.0;
    if (value > 1.0) normalizedValue = 1.0;
    const double thresholded = static_cast<double>(static_cast<int>(
            normalizedValue * channels))
        / channels;
    int intVal = static_cast<int>(std::lround(thresholded * 255.0));
    intVal = std::clamp(intVal, 0, 255);
    return static_cast<std::byte>(static_cast<uint8_t>(intVal));
}

void write_n_bytes(std::vector<std::byte>& vec, const int n, const int data)
{
    // Increasing offset due to little-endian ordering (LSB first)
    int offset = 0;
    for (int i = 0; i < n; i++)
    {
        vec.push_back(static_cast<std::byte>(data >> offset & 255));
        offset += 8;
    }
}

void Image::validate_dimensions(const int x, const int y) const
{
    if (x >= height) throw std::invalid_argument("Height exceeded");
    if (y >= width) throw std::invalid_argument("Width exceeded");
}

Image::Image(const int _width, const int _height, const int _channels, const bool _is_grayscale) :
    width(_width), height(_height), color_channels(_channels), is_grayscale(_is_grayscale)
{
    for (int i = 0; i < _width * _height; i++)
    {
        data.push_back(Vec3::ZERO);
    }
}

Image::Image(const Vec3& color) :
    width(1), height(1), color_channels(256), is_grayscale(false)
{
    data.push_back(color);
}

void Image::draw(const int x, const int y, const Vec3& color)
{
    validate_dimensions(x, y);
    data[x * width + y] = color;
}

void Image::add_color(const int x, const int y, const Vec3& color)
{
    validate_dimensions(x, y);
    data[x * width + y] += color;
}

void Image::generateBmp(const std::string& file_name) const
{
    // Writing the header
    std::vector<std::byte> image_bytes;

    // File Header signature
    image_bytes.push_back(static_cast<std::byte>('B'));
    image_bytes.push_back(static_cast<std::byte>('M'));

    const int file_size = 14 + 40 + 3 * width * height;
    write_n_bytes(image_bytes, 4, file_size);

    constexpr int reserved_field = 0;
    write_n_bytes(image_bytes, 4, reserved_field);

    constexpr int pixel_data_offset = 14 + 40;
    write_n_bytes(image_bytes, 4, pixel_data_offset);

    constexpr int bitmap_header_size = 40;
    write_n_bytes(image_bytes, 4, bitmap_header_size);

    write_n_bytes(image_bytes, 4, width);
    write_n_bytes(image_bytes, 4, height);

    constexpr int bmp_reserved_field = 1;
    write_n_bytes(image_bytes, 2, bmp_reserved_field);

    constexpr int bits_per_pixel = 24;
    write_n_bytes(image_bytes, 2, bits_per_pixel);

    constexpr int compression = 0;
    write_n_bytes(image_bytes, 4, compression);

    const int size_of_pixel_data = 3 * width * height;
    write_n_bytes(image_bytes, 4, size_of_pixel_data);

    constexpr int horizontal_resolution = 2835;
    write_n_bytes(image_bytes, 4, horizontal_resolution);

    constexpr int vertical_resolution = 2835;
    write_n_bytes(image_bytes, 4, vertical_resolution);

    constexpr int color_palette_info = 0;
    write_n_bytes(image_bytes, 4, color_palette_info);

    constexpr int no_important_colors = 0;
    write_n_bytes(image_bytes, 4, no_important_colors);

    for (int r = height - 1; r >= 0; r--)
    {
        for (int c = 0; c < width; c++)
        {
            const int i = r * width + c;

            if (is_grayscale)
            {
                const double gray = (data[i].x + data[i].y + data[i].z) / 3;
                const std::byte gray_byte = doubleToColorByte(gray, color_channels);
                image_bytes.push_back(gray_byte);
                image_bytes.push_back(gray_byte);
                image_bytes.push_back(gray_byte);
            }
            else
            {
                image_bytes.push_back(doubleToColorByte(data[i].z, color_channels));
                image_bytes.push_back(doubleToColorByte(data[i].y, color_channels));
                image_bytes.push_back(doubleToColorByte(data[i].x, color_channels));
            }
        }
        int bytes_so_far = width * 3;
        while (bytes_so_far % 4 != 0)
        {
            image_bytes.push_back(static_cast<std::byte>(0));
            bytes_so_far++;
        }
    }

    // std::ios::binary needed for binary data instead of text data on Windows
    std::ofstream image_file(file_name, std::ios::binary);

    image_file.write(reinterpret_cast<const char*>(image_bytes.data()), static_cast<long long>(image_bytes.size()));
    image_file.close();
}
