#pragma once
#include <vector>
#include "util/vec3.h"

namespace raytracer::common
{
/**
 * A vector of colors.
 */
class Image
{
    std::vector<Vec3> data;
    const int width, height;
    const int color_channels;
    const bool is_grayscale;

    void validate_dimensions(int x, int y) const;

  public:
    /**
     * Creates an image with the given properties and fills it with zero vectors (black).
     *
     * @param _width The width of the image.
     * @param _height The height of the image.
     * @param _channels The number of color channels of the image.
     * @param _is_grayscale Whether the image is grayscale (scalar color instead of RGB).
     */
    Image(int _width, int _height, int _channels, bool _is_grayscale);

    /**
     * Creates a 1x1 image with the given color.
     *
     * @param color The color of the sole pixel.
     */
    explicit Image(const Vec3 &color);

    /**
     * Sets the color of the image at a given position.
     *
     * @param x The row of the image, starting from the top.
     * @param y The column of the image, starting from the left.
     * @param color The new color at that position.
     */
    void draw(int x, int y, const Vec3 &color);

    /**
     * Adds a color vector on top of the existing one at a given position.
     *
     * @param x The row of the image, starting from the top.
     * @param y The column of the image, starting from the left.
     * @param color The color to add on top.
     */
    void add_color(int x, int y, const Vec3 &color);

    /**
     * Write the pixel data to a bitmap on disk.
     *
     * @param file_name The name of the file to write to.
     */
    void generateBmp(const std::string &file_name) const;

    /**
     * Getter for the width of the image.
     *
     * @return The width of the image.
     */
    [[nodiscard]] int get_width() const
    {
        return width;
    }

    /**
     * Getter for the height of the image.
     *
     * @return The height of the image.
     */
    [[nodiscard]] int get_height() const
    {
        return height;
    }

    /**
     * Getter for the number of color channels the image has.
     *
     * @return The number of color channels the image has.
     */
    [[nodiscard]] int get_color_channels() const
    {
        return color_channels;
    }

    /**
     * Getter for whether the image is grayscale.
     *
     * @return True if the image is grayscale, false otherwise.
     */
    [[nodiscard]] bool get_is_grayscale() const
    {
        return is_grayscale;
    }

    /**
     * Returns a const reference to the underlying vector of colors.
     *
     * @return A const reference to the underlying vector of colors.
     */
    [[nodiscard]] const std::vector<Vec3> &get_data() const
    {
        return data;
    }

    /**
     * Returns the color at the given position.
     *
     * @param x The row of the image, starting from the top.
     * @param y The column of the image, starting from the left.
     * @return The color at the given position.
     */
    [[nodiscard]] Vec3 get_color(const int x, const int y) const
    {
        return data[x * width + y];
    }
};
} // namespace raytracer::common
