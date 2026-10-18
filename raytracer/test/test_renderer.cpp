#include "raytracer/renderer.h"
#include <algorithm>
#include <gtest/gtest.h>

#include "common/image_similarity.h"
#include "common/util/random_utils.h"
#include "raytracer/materials/lambertian.h"
#include "raytracer/materials/lambertian_texture.h"
#include "raytracer/materials/metal.h"
#include "raytracer/shapes/floor.h"
#include "raytracer/shapes/sphere.h"

namespace raytracer::raytracer
{
static Renderer renderer;
static const bool IS_SEED_SET = common::set_seed(common::TEST_SEED);

static constexpr double TOLERANCE = 0.02; // Twice the experimental diff value for two completely identical scenes

static constexpr common::Vec3 BACKGROUND_COLOR{0.39, 0.582, 0.9258};
static constexpr double FOCAL_LENGTH = 1.0;
static constexpr int SCREEN_WIDTH_PIXELS = 512;
static constexpr int SCREEN_HEIGHT_PIXELS = 512;
static constexpr int RAYS_PER_PIXEL = 4;
static constexpr int MAX_RECURSION_DEPTH = 50;

static constexpr common::Vec3 YELLOW{1, 1, 0};
static constexpr common::Vec3 SPHERE_CENTER{0, -5, -20};
static constexpr double SPHERE_RADIUS = 5;
static constexpr double FLOOR_Y = -10;
static const Metal METAL{common::Vec3{0.4, 0.4, 0.8}, 0.6};

static std::unique_ptr<Object> make_sphere(const common::Vec3 &center, const double radius,
                                           const common::Vec3 &color = YELLOW)
{
    return std::make_unique<Sphere>(std::make_unique<const Lambertian>(color), center, radius);
}

static std::unique_ptr<Object> make_floor(const double y)
{
    return std::make_unique<Floor>(std::make_unique<const Metal>(METAL), y);
}

static std::vector<std::unique_ptr<Object>> make_default_objects()
{
    std::vector<std::unique_ptr<Object>> objects;
    objects.emplace_back(make_sphere(SPHERE_CENTER, SPHERE_RADIUS));
    objects.emplace_back(make_floor(FLOOR_Y));
    return objects;
}

static RayTracerSceneConfig make_scene(std::vector<std::unique_ptr<Object>> &&objects,
                                       const common::Vec3 &camera_origin = common::Vec3::ZERO,
                                       const common::Vec3 &camera_direction = {0, 0, -1})
{
    return {.background_color = BACKGROUND_COLOR,
            .sky_color = common::Vec3{1, 1, 1},
            .camera{-1.0, 1.0, -1.0, 1.0, FOCAL_LENGTH, camera_origin, camera_direction, common::Vec3{0, 1, 0},
                    SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, ViewType::PERSPECTIVE, 256, RAYS_PER_PIXEL,
                    MAX_RECURSION_DEPTH},
            .objects = std::move(objects),
            .is_shading_thresholded = false,
            .thresholded_shading_degrees = 1,
            .is_grayscale = false,
            .output_path = ""};
}

TEST(RayTracerRenderTest, RotatingTheSceneAndCameraTogether)
{
    // Rotate by 90 degrees around the Y axis, mapping (x, y, z) to (z, y, -x). The horizontal floor is unaffected.
    const auto rotate = [](const common::Vec3 &v) { return common::Vec3{v.z, v.y, -v.x}; };
    const auto img_original = renderer.render(make_scene(make_default_objects()));

    std::vector<std::unique_ptr<Object>> objects_rotated;
    objects_rotated.emplace_back(make_sphere(rotate(SPHERE_CENTER), SPHERE_RADIUS));
    objects_rotated.emplace_back(make_floor(FLOOR_Y));
    const auto img_rotated =
        renderer.render(make_scene(std::move(objects_rotated), common::Vec3::ZERO, rotate({0, 0, -1})));

    EXPECT_NEAR(0.0, common::diff_images(img_original, img_rotated), TOLERANCE);
}

TEST(RayTracerRenderTest, TranslatingTheSceneAndCameraTogether)
{
    constexpr common::Vec3 offset{100000, 100000, 100000};
    const auto img_original = renderer.render(make_scene(make_default_objects()));

    std::vector<std::unique_ptr<Object>> objects_translated;
    objects_translated.emplace_back(make_sphere(SPHERE_CENTER + offset, SPHERE_RADIUS));
    objects_translated.emplace_back(make_floor(FLOOR_Y + offset.y));
    const auto img_translated = renderer.render(make_scene(std::move(objects_translated), offset));

    EXPECT_NEAR(0.0, common::diff_images(img_original, img_translated), TOLERANCE);
}

TEST(RayTracerRenderTest, LambertianIsSameAsLambertianTextureWithConstantColor)
{
    const auto img_original = renderer.render(make_scene(make_default_objects()));

    std::vector<std::unique_ptr<Object>> objects_textured;
    objects_textured.emplace_back(
        std::make_unique<Sphere>(std::make_unique<const LambertianTexture>(
                                     Cubemap(common::Image(YELLOW), common::Image(YELLOW), common::Image(YELLOW),
                                             common::Image(YELLOW), common::Image(YELLOW), common::Image(YELLOW))),
                                 SPHERE_CENTER, SPHERE_RADIUS));
    objects_textured.emplace_back(make_floor(FLOOR_Y));
    const auto img_textured = renderer.render(make_scene(std::move(objects_textured)));

    EXPECT_EQ(0.0, common::diff_images(img_original, img_textured));
}

TEST(RayTracerRenderTest, AddingAnObjectBehindTheCamera)
{
    const auto img_original = renderer.render(make_scene(make_default_objects()));

    auto objects_with_occluded = make_default_objects();
    objects_with_occluded.emplace_back(make_sphere({0, -5, 20}, SPHERE_RADIUS));
    const auto img_with_occluded = renderer.render(make_scene(std::move(objects_with_occluded)));

    EXPECT_NEAR(0.0, common::diff_images(img_original, img_with_occluded), TOLERANCE);
}

TEST(RayTracerRenderTest, AddingAnObjectInsideAnotherObject)
{
    const auto img_original = renderer.render(make_scene(make_default_objects()));

    auto objects_with_occluded = make_default_objects();
    objects_with_occluded.emplace_back(make_sphere(SPHERE_CENTER, SPHERE_RADIUS - 1, {0, 1, 1}));
    const auto img_with_occluded = renderer.render(make_scene(std::move(objects_with_occluded)));

    EXPECT_EQ(0.0, common::diff_images(img_original, img_with_occluded));
}

TEST(RayTracerRenderTest, ReorderingTheSceneObjectList)
{
    const auto img_original = renderer.render(make_scene(make_default_objects()));

    auto objects_with_reordering = make_default_objects();
    std::ranges::reverse(objects_with_reordering);
    const auto img_with_reordering = renderer.render(make_scene(std::move(objects_with_reordering)));

    EXPECT_EQ(0.0, common::diff_images(img_original, img_with_reordering));
}

TEST(RayTracerRenderTest, SameSceneWithVaryingThreadCount)
{
    std::vector<common::Image> images;
    for (const auto thread_count : {1, 2, 4, 8})
    {
        images.emplace_back(renderer.render(make_scene(make_default_objects()), common::get_nullout(), thread_count));
        if (thread_count > 1)
        {
            EXPECT_NEAR(0.0, common::diff_images(images[images.size() - 1], images[images.size() - 2]), TOLERANCE)
                << "thread counts: " << thread_count / 2 << ' ' << thread_count;
        }
    }
}
} // namespace raytracer::raytracer
