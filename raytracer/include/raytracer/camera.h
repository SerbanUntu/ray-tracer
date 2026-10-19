#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include "common/util/vec3.h"

namespace raytracer::raytracer
{
enum class ViewType : uint8_t
{
    ORTHOGRAPHIC,
    PERSPECTIVE
};

NLOHMANN_JSON_SERIALIZE_ENUM(ViewType,
                             {{ViewType::ORTHOGRAPHIC, "orthographic"}, {ViewType::PERSPECTIVE, "perspective"}})

struct Ray
{
    common::Vec3 origin;
    common::Vec3 direction;
};

struct Pixel
{
    int x; // column [0, screen_width_pixels)
    int y; // row    [0, screen_height_pixels)
};

class CameraBuilder;

class Camera
{
    friend CameraBuilder;

    double screen_left_coord = -1.0;
    double screen_right_coord = 1.0;
    double screen_bottom_coord = -1.0;
    double screen_top_coord = 1.0;
    double focal_length = 1.0;
    common::Vec3 origin = common::Vec3::ZERO;
    common::Vec3 direction{0, 0, -1};
    common::Vec3 world_up{0, 1, 0};
    int screen_width_pixels = 1920;
    int screen_height_pixels = 1080;
    ViewType view_type = ViewType::PERSPECTIVE;
    int color_channels = 256;
    int rays_per_pixel = 4;
    int max_recursion_depth = 32;

  public:
    Camera() = default;
    Camera(double _screen_left_coord, double _screen_right_coord, double _screen_bottom_coord, double _screen_top_coord,
           double _focal_length, const common::Vec3 &_origin, const common::Vec3 &_direction,
           const common::Vec3 &_world_up, int _screen_width_pixels, int _screen_height_pixels, ViewType _view_type,
           int _color_channels, int _rays_per_pixel, int _max_recursion_depth);
    /**
     * Compute the direction that points to a pixel center (with jitter), from the camera origin.
     *
     * @param p The pixel defined by its x and y coordinates in the screen plane.
     * @return The ray with the origin at the camera, pointed towards the pixel.
     */
    [[nodiscard]] Ray compute_ray_for_pixel(Pixel p) const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Camera, screen_left_coord, screen_right_coord, screen_bottom_coord, screen_top_coord,
                                   focal_length, origin, direction, world_up, screen_width_pixels, screen_height_pixels,
                                   view_type, color_channels, rays_per_pixel, max_recursion_depth)

    /**
     * Getter for the width of the rendered image in pixels.
     *
     * @return The width of the rendered image in pixels.
     */
    [[nodiscard]] int get_screen_width_pixels() const
    {
        return screen_width_pixels;
    }

    /**
     * Getter for the height of the rendered image in pixels.
     *
     * @return The height of the rendered image in pixels.
     */
    [[nodiscard]] int get_screen_height_pixels() const
    {
        return screen_height_pixels;
    }

    /**
     * Getter for the number of color channels of the rendered image.
     *
     * @return The number of color channels of the rendered image.
     */
    [[nodiscard]] int get_color_channels() const
    {
        return color_channels;
    }

    /**
     * Getter for the number of rays traced per pixel, whose colors are averaged.
     *
     * @return The number of rays traced per pixel.
     */
    [[nodiscard]] int get_rays_per_pixel() const
    {
        return rays_per_pixel;
    }

    /**
     * Getter for the maximum number of times a ray can bounce before tracing stops.
     *
     * @return The maximum number of times a ray can bounce before tracing stops.
     */
    [[nodiscard]] int get_max_recursion_depth() const
    {
        return max_recursion_depth;
    }
};

class CameraBuilder
{
    Camera camera;

  public:
    CameraBuilder();
    CameraBuilder &with_screen_dimensions(int width, int height) &;
    CameraBuilder &with_focal_length(double focal_length) &;
    CameraBuilder &with_origin(const common::Vec3 &origin) &;
    CameraBuilder &with_direction(const common::Vec3 &direction) &;
    CameraBuilder &with_world_up(const common::Vec3 &direction) &;
    CameraBuilder &with_virtual_screen_boundaries(double left, double top, double right, double bottom) &;
    CameraBuilder &with_view_type(ViewType view_type) &;
    CameraBuilder &with_color_channels(int color_channels) &;
    CameraBuilder &with_rays_per_pixel(int rays_per_pixel) &;
    CameraBuilder &with_max_recursion_depth(int max_recursion_depth) &;
    [[nodiscard]] Camera build() const;
};
} // namespace raytracer::raytracer
