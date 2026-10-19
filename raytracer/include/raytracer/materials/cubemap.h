#pragma once
#include <cstdint>
#include <cstddef>
#include "common/image.h"

namespace raytracer::raytracer
{
enum class TextureExtension : uint8_t
{
    REPEAT,
    CLAMP,
    CONSTANT
};

/**
 * Reads a BGR pixel from a bitmap buffer and draws it to the given position of a cubemap face.
 *
 * @param i The row of the face, starting from the top.
 * @param j The column of the face, starting from the left.
 * @param pos The byte offset of the pixel in the buffer.
 * @param buffer The raw bytes of the bitmap file.
 * @param img The face to draw to.
 */
void draw_face(int i, int j, int pos, const std::vector<std::byte> &buffer, common::Image *img);

/**
 * A texture made of six images, one for each face of a cube, which is sampled using a direction vector.
 */
class Cubemap
{
    common::Image top, left, front, right, back, bottom;

  public:
    /**
     * Loads a cubemap from a bitmap on disk, with the faces laid out in a 4x3 cross.
     *
     * @throws std::runtime_error If the file cannot be opened or read, or if it is too small for the face dimensions.
     *
     * @param path The path to the bitmap file.
     * @param face_width The width of each face in pixels.
     * @param face_height The height of each face in pixels.
     * @param color_channels The number of color channels of each face.
     * @param is_grayscale Whether the faces are grayscale (scalar color instead of RGB).
     */
    Cubemap(const std::string &path, int face_width, int face_height, int color_channels, bool is_grayscale);

    /**
     * Creates a cubemap from six existing images.
     *
     * @param _top The top face.
     * @param _left The left face.
     * @param _front The front face.
     * @param _right The right face.
     * @param _back The back face.
     * @param _bottom The bottom face.
     */
    Cubemap(common::Image _top, common::Image _left, common::Image _front, common::Image _right, common::Image _back,
            common::Image _bottom);

    /**
     * Calculates the texture color at a point on the sphere, given the normal.
     *
     * @param normal The normal to the sphere surface at that point.
     * @return A vector representing the color at that point.
     */
    [[nodiscard]] common::Vec3 get_color_at_point(const common::Vec3 &normal) const;
};
} // namespace raytracer::raytracer
