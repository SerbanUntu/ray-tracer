#include "mandelbrot/renderer.h"

#include <numbers>

namespace raytracer::mandelbrot
{
using namespace std::chrono;

constexpr auto STDOUT_REFRESH_INTERVAL = 100ms;

// NOLINTBEGIN(modernize-use-std-numbers)
constexpr std::array PALETTE = {common::Vec3(0.094118, 0.321569, 0.694118), common::Vec3(0.223529, 0.490196, 0.819608),
                                common::Vec3(0.525490, 0.709804, 0.898039), common::Vec3(0.827451, 0.925490, 0.972549),
                                common::Vec3(0.945098, 0.913725, 0.749020), common::Vec3(0.972549, 0.788235, 0.372549),
                                common::Vec3(1.000000, 0.666667, 0.000000), common::Vec3(0.800000, 0.501961, 0.000000),
                                common::Vec3(0.600000, 0.341176, 0.000000), common::Vec3(0.415686, 0.203922, 0.011765),
                                common::Vec3(0.258824, 0.117647, 0.058824), common::Vec3(0.098039, 0.027451, 0.101961),
                                common::Vec3(0.035294, 0.003922, 0.184314), common::Vec3(0.015686, 0.015686, 0.286275),
                                common::Vec3(0.000000, 0.027451, 0.392157), common::Vec3(0.047059, 0.172549, 0.541176)};
// NOLINTEND(modernize-use-std-numbers)

double Renderer::calculate_iterations(const common::Complex c, const int escape_boundary_squared,
                                      const int max_iterations)
{
    double i = 0;
    common::Complex z = common::Complex::ZERO;
    while (z.magnitude_squared() < escape_boundary_squared && i < max_iterations)
    {
        z = z.squared() + c;
        i++;
    }

    if (i >= max_iterations)
        return i;

    // Smoothing
    const double log_zn = std::log(z.magnitude_squared()) / 2.;
    constexpr double log_2 = std::numbers::ln2;
    const double nu = std::log(log_zn / log_2) / log_2;
    return i + 1 - nu;
}

common::Vec3 Renderer::get_mandelbrot_color(const double iterations, const int max_iterations,
                                            const std::span<const common::Vec3> palette)
{
    if (std::isnan(iterations) || iterations >= max_iterations)
        return common::Vec3::ZERO;

    const int integer = static_cast<int>(std::floor(iterations));
    const double decimal = iterations - static_cast<double>(integer);
    // The smoothing term can push `iterations` below zero, and C++ modulo keeps the sign
    const int index = ((integer % palette.size()) + palette.size()) % palette.size();
    const int next_index = (index + 1) % palette.size();
    return palette[index] + (palette[next_index] - palette[index]) * decimal;
}

common::Complex Renderer::get_coordinate(const int row, const int col, const MandelbrotSceneSpace &mss)
{
    // Sample pixel centres, so that a 1-pixel-wide or 1-pixel-tall image does not divide by zero
    double re =
        mss.left + (((static_cast<double>(col) + .5) / static_cast<double>(mss.width_pixels)) * (mss.right - mss.left));
    double im =
        mss.top + (((static_cast<double>(row) + .5) / static_cast<double>(mss.height_pixels)) * (mss.bottom - mss.top));
    return {re, im};
}

common::Image Renderer::render(const MandelbrotSceneConfig &scene, std::ostream &out, const int number_of_threads)
{
    const int HEIGHT = static_cast<int>(static_cast<double>(scene.width) / scene.aspect_ratio);
    const double LEFT = (-scene.aspect_ratio / scene.zoom) + scene.center.x;
    const double RIGHT = (scene.aspect_ratio / scene.zoom) + scene.center.x;
    const double BOTTOM = (-1. / scene.zoom) + scene.center.y;
    const double TOP = (1 / scene.zoom) + scene.center.y;

    auto img = common::Image(scene.width, HEIGHT, 256, false);
    const MandelbrotSceneSpace mss(LEFT, RIGHT, BOTTOM, TOP, HEIGHT, scene.width, scene.center);

    out << "Rendering the mandelbrot set...\n";
    {
        std::atomic finished_rows = 0;

        std::jthread progress_thread{[&finished_rows, &out, HEIGHT] {
            while (true)
            {
                const int current_row = finished_rows.load();
                common::clear_current_stdout_row(out);
                common::display_percentage(out, current_row, HEIGHT, "row");
                if (current_row >= HEIGHT)
                    return;
                std::this_thread::sleep_for(STDOUT_REFRESH_INTERVAL);
            }
        }};

        std::vector<std::jthread> threads;
        threads.reserve(number_of_threads);
        for (int thread_idx = 0; thread_idx < number_of_threads; thread_idx++)
        {
            threads.emplace_back([&, thread_idx] {
                for (int row = thread_idx; row < HEIGHT; row += number_of_threads)
                {
                    for (int col = 0; col < scene.width; col++)
                    {
                        img.draw(row, col,
                                 get_mandelbrot_color(calculate_iterations(get_coordinate(row, col, mss),
                                                                           scene.escape_boundary_squared,
                                                                           scene.max_iterations),
                                                      scene.max_iterations, PALETTE));
                    }
                    ++finished_rows;
                }
            });
        }
    }
    common::clear_current_stdout_row(out);
    common::display_percentage(out, HEIGHT, HEIGHT, "row");
    return img;
}
} // namespace raytracer::mandelbrot