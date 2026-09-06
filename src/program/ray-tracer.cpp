#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "image.h"
#include "scene.h"
#include "shapes/floor.h"
#include "shapes/sphere.h"
#include "util/vec3.h"
#include "util/terminal.h"
#include "materials/lambertian.h"
#include "materials/lambertian_texture.h"
#include "materials/metal.h"
#include "materials/cubemap.h"
#include "camera.h"

using namespace std::chrono;

constexpr auto STDOUT_REFRESH_INTERVAL = 100ms;

constexpr auto MIN_INTERSECTION_DISTANCE = 0.001;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vec3, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Camera, screen_left_coord, screen_right_coord, screen_bottom_coord, screen_top_coord,
                                   focal_length, origin, direction, world_up, screen_width_pixels, screen_height_pixels,
                                   view_type, color_channels, rays_per_pixel, max_recursion_depth)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RayTracerSceneConfig, background_color, sky_color, camera, objects,
                                   is_shading_thresholded, thresholded_shading_degrees, is_grayscale, output_path)
NLOHMANN_JSON_SERIALIZE_ENUM(ViewType, {
                             {ViewType::ORTHOGRAPHIC, "orthographic"},
                             {ViewType::PERSPECTIVE, "perspective"}
                             })

static Vec3 shade(const Object& o, const Vec3& intersection, const Ray& r, int depth, int max_depth,
                  const RayTracerSceneConfig& scene);

static Vec3 trace(const Ray& r, const int depth, const int max_depth, const RayTracerSceneConfig& scene)
{
    if (depth > max_depth) return Vec3::ZERO;

    double min_depth = std::numeric_limits<double>::max();
    Vec3 top_color = depth == 0 ? scene.background_color : scene.sky_color;

    for (const auto& oPtr : scene.objects)
    {
        const Object& o = *oPtr;
        const double t = o.ray_intersection(r);
        if (t < min_depth && t > MIN_INTERSECTION_DISTANCE)
        {
            min_depth = t;
            top_color = shade(o, r.origin + r.direction * t, r, depth, max_depth, scene);
        }
    }

    return top_color;
}

static Vec3 shade(const Object& o, const Vec3& intersection, const Ray& r, const int depth, const int max_depth,
                  const RayTracerSceneConfig& scene)
{
    const Material* mat = o.get_material();

    const Vec3 normal = o.get_normal(intersection);
    const Vec3 rec = trace(mat->get_scattered(r, intersection, normal), depth + 1, max_depth, scene);
    const Vec3 col = mat->get_color(r, intersection, normal);

    return Vec3(rec.x * col.x, rec.y * col.y, rec.z * col.z);
}

int main()
{
    std::vector<std::unique_ptr<Object>> objects;
    objects.emplace_back(
        std::make_unique<Sphere>(
            std::make_unique<const Lambertian>(Vec3(1, 1, 0)),
            Vec3(6, -5, -20),
            6
        )
    );
    objects.emplace_back(
        std::make_unique<Sphere>(
            std::make_unique<const Metal>(Vec3(0.4, 0.5, 0.6), 0.1),
            Vec3(-6, -5, -20),
            6
        )
    );
    objects.emplace_back(
        std::make_unique<Sphere>(
            std::make_unique<const LambertianTexture>(
                Cubemap(
                    Image(Vec3(1, 0, 0)),
                    Image(Vec3(0, 1, 0)),
                    Image(Vec3(0, 0, 1)),
                    Image(Vec3(1, 1, 0)),
                    Image(Vec3(0, 1, 1)),
                    Image(Vec3(1, 0, 1))
                )
            ),
            Vec3(0, 10, -120),
            20
        )
    );
    objects.emplace_back(
        std::make_unique<Floor>(
            std::make_unique<const Metal>(Vec3(0.4, 0.4, 0.8), 0.6),
            -11
        )
    );

    // Scene config
    const RayTracerSceneConfig scene(
        Vec3(0.39, 0.582, 0.9258),
        Vec3(1, 1, 1),
        Camera(
            -1.0, 1.0,
            -1.0,
            1.0,
            1.0,
            Vec3::ZERO,
            Vec3(0, 0, -1),
            Vec3(0, 1, 0),
            800,
            800,
            ViewType::PERSPECTIVE,
            256,
            4,
            50
        ),
        std::move(objects),
        false,
        4.0,
        false,
        "ray-tracer.bmp"
    );

    auto img = Image(
        scene.camera.screen_width_pixels,
        scene.camera.screen_height_pixels,
        scene.camera.color_channels,
        scene.is_grayscale
    );

    std::cout << "Rendering the scene...\n";
    {
        std::atomic finished_rows = 0;
        const size_t number_of_threads = std::max(1u, std::thread::hardware_concurrency());

        std::jthread progress_thread{
            [&finished_rows, &scene]
            {
                while (true)
                {
                    const int current_row = finished_rows.load();
                    clear_current_stdout_row();
                    display_percentage(current_row, scene.camera.screen_height_pixels, "row");
                    if (current_row >= scene.camera.screen_height_pixels) return;
                    std::this_thread::sleep_for(STDOUT_REFRESH_INTERVAL);
                }
            }
        };

        std::vector<std::jthread> threads;
        threads.reserve(number_of_threads);
        for (int thread_idx = 0; thread_idx < number_of_threads; thread_idx++)
        {
            threads.emplace_back([&, thread_idx]
            {
                for (int row = thread_idx; row < scene.camera.screen_height_pixels; row += number_of_threads)
                {
                    for (int col = 0; col < scene.camera.screen_width_pixels; col++)
                    {
                        auto pixel_color = Vec3(0, 0, 0);

                        for (int k = 0; k < scene.camera.rays_per_pixel; k++)
                        {
                            const Ray current_ray = scene.camera.compute_ray_for_pixel(Pixel(col, row));
                            pixel_color += trace(current_ray, 0, scene.camera.max_recursion_depth, scene);
                        }

                        img.draw(row, col, pixel_color / scene.camera.rays_per_pixel);
                    }
                    ++finished_rows;
                }
            });
        }
    }
    clear_current_stdout_row();
    display_percentage(scene.camera.screen_height_pixels, scene.camera.screen_height_pixels, "row");

    std::cout << "\nRendered!\n\nSaving to " << scene.output_path << "...";
    img.generateBmp(scene.output_path);
    std::cout << "\nSaved!\n";
    return 0;
}
