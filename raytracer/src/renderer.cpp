#include "raytracer/renderer.h"

namespace raytracer::raytracer
{
using namespace std::chrono;

constexpr auto STDOUT_REFRESH_INTERVAL = 100ms;
constexpr auto MIN_INTERSECTION_DISTANCE = 0.001;

common::Vec3 Renderer::shade(const Object &o, const common::Vec3 &intersection, const Ray &r, const int depth,
                             const int max_depth, const RayTracerSceneConfig &scene)
{
    const Material *mat = o.get_material();

    const common::Vec3 normal = o.get_normal(intersection);
    const common::Vec3 rec = trace(mat->get_scattered(r, intersection, normal), depth + 1, max_depth, scene);
    const common::Vec3 col = mat->get_color(r, intersection, normal);

    return {rec.x * col.x, rec.y * col.y, rec.z * col.z};
}

common::Vec3 Renderer::trace(const Ray &r, const int depth, const int max_depth, const RayTracerSceneConfig &scene)
{
    if (depth > max_depth)
        return common::Vec3::ZERO;

    double min_depth = std::numeric_limits<double>::max();
    common::Vec3 top_color = depth == 0 ? scene.background_color : scene.sky_color;

    for (const auto &oPtr : scene.objects)
    {
        const Object &o = *oPtr;
        if (const double t = o.ray_intersection(r); t < min_depth && t > MIN_INTERSECTION_DISTANCE)
        {
            min_depth = t;
            top_color = shade(o, r.origin + r.direction * t, r, depth, max_depth, scene);
        }
    }

    return top_color;
}

common::Image Renderer::render(const RayTracerSceneConfig &scene, std::ostream &out, const int number_of_threads)
{

    auto img = common::Image(scene.camera.screen_width_pixels, scene.camera.screen_height_pixels,
                             scene.camera.color_channels, scene.is_grayscale);

    out << "Rendering the scene...\n";
    {
        std::atomic finished_rows = 0;

        std::jthread progress_thread{[&finished_rows, &scene, &out] {
            while (true)
            {
                const int current_row = finished_rows.load();
                common::clear_current_stdout_row(out);
                common::display_percentage(out, current_row, scene.camera.screen_height_pixels, "row");
                if (current_row >= scene.camera.screen_height_pixels)
                    return;
                std::this_thread::sleep_for(STDOUT_REFRESH_INTERVAL);
            }
        }};

        std::vector<std::jthread> threads;
        threads.reserve(number_of_threads);
        for (int thread_idx = 0; thread_idx < number_of_threads; thread_idx++)
        {
            threads.emplace_back([&, thread_idx] {
                for (int row = thread_idx; row < scene.camera.screen_height_pixels; row += number_of_threads)
                {
                    for (int col = 0; col < scene.camera.screen_width_pixels; col++)
                    {
                        auto pixel_color = common::Vec3(0, 0, 0);

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
    common::clear_current_stdout_row(out);
    common::display_percentage(out, scene.camera.screen_height_pixels, scene.camera.screen_height_pixels, "row");
    return img;
}
} // namespace raytracer::raytracer
