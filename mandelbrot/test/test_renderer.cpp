#include "mandelbrot/renderer.h"
#include <gtest/gtest.h>

#include "common/image_similarity.h"

namespace raytracer::mandelbrot
{
static Renderer renderer;
static constexpr int MAX_ITERATIONS = 1000;
static constexpr int ESCAPE_BOUNDARY_SQUARED = 256;

static constexpr int WIDTH = 512;
static constexpr double ASPECT_RATIO = 2;
static constexpr double ZOOM = 8;
static constexpr common::Point CENTER{.x = 0.0, .y = 0.75};

static MandelbrotSceneConfig make_scene(const common::Point center, const int width, const double aspect_ratio,
                                        const double zoom)
{
    return {.center = center,
            .width = width,
            .aspect_ratio = aspect_ratio,
            .zoom = zoom,
            .max_iterations = MAX_ITERATIONS,
            .escape_boundary_squared = ESCAPE_BOUNDARY_SQUARED,
            .output_path = ""};
}

static void copy_image(common::Image &destination, const common::Image &source, const int row_offset,
                       const int col_offset)
{
    for (int row = 0; row < source.get_height(); row++)
        for (int col = 0; col < source.get_width(); col++)
            destination.draw(row_offset + row, col_offset + col, source.get_color(row, col));
}

TEST(MandelbrotRenderTest, ComposingAnImageMadeOfTwoIdenticalParts)
{
    const auto img_full = renderer.render(make_scene(CENTER, WIDTH, ASPECT_RATIO, ZOOM));
    constexpr double half_span = 1. / ZOOM;
    const auto img_left = renderer.render(make_scene({.x = CENTER.x - half_span, .y = CENTER.y}, WIDTH / 2, 1, ZOOM));
    const auto img_right = renderer.render(make_scene({.x = CENTER.x + half_span, .y = CENTER.y}, WIDTH / 2, 1, ZOOM));

    common::Image img_combined{WIDTH, img_left.get_height(), img_left.get_color_channels(),
                               img_left.get_is_grayscale()};
    copy_image(img_combined, img_left, 0, 0);
    copy_image(img_combined, img_right, 0, img_left.get_width());

    EXPECT_EQ(0.0, common::diff_images(img_full, img_combined, 1));
}

TEST(MandelbrotRenderTest, ComposingAnImageMadeOfFourIdenticalParts)
{
    static constexpr double HALF_SPAN_X = ASPECT_RATIO / ZOOM / 2;
    static constexpr double HALF_SPAN_Y = 1. / ZOOM / 2;
    const auto make_quadrant = [](const double dx, const double dy) {
        return renderer.render(make_scene({.x = CENTER.x + dx, .y = CENTER.y + dy}, WIDTH / 2, ASPECT_RATIO, ZOOM * 2));
    };
    const auto img_full = renderer.render(make_scene(CENTER, WIDTH, ASPECT_RATIO, ZOOM));
    const auto img_top_left = make_quadrant(-HALF_SPAN_X, HALF_SPAN_Y);
    const auto img_top_right = make_quadrant(HALF_SPAN_X, HALF_SPAN_Y);
    const auto img_bottom_left = make_quadrant(-HALF_SPAN_X, -HALF_SPAN_Y);
    const auto img_bottom_right = make_quadrant(HALF_SPAN_X, -HALF_SPAN_Y);

    common::Image img_combined{img_full.get_width(), img_full.get_height(), img_full.get_color_channels(),
                               img_full.get_is_grayscale()};
    copy_image(img_combined, img_top_left, 0, 0);
    copy_image(img_combined, img_top_right, 0, img_top_left.get_width());
    copy_image(img_combined, img_bottom_left, img_top_left.get_height(), 0);
    copy_image(img_combined, img_bottom_right, img_top_left.get_height(), img_top_left.get_width());

    EXPECT_EQ(0.0, common::diff_images(img_full, img_combined, 1));
}
} // namespace raytracer::mandelbrot
