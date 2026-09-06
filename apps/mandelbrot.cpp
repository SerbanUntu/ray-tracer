#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <nlohmann/json.hpp>
#include "common/image.h"
#include "common/util/complex.h"
#include "common/util/terminal.h"
#include "mandelbrot/scene.h"

using namespace std::chrono;

constexpr auto STDOUT_REFRESH_INTERVAL = 100ms;
constexpr auto PALETTE_SIZE = 16;
constexpr Vec3 PALETTE[PALETTE_SIZE] = {
    Vec3(0.094118, 0.321569, 0.694118), Vec3(0.223529, 0.490196, 0.819608), Vec3(0.525490, 0.709804, 0.898039),
    Vec3(0.827451, 0.925490, 0.972549),
    Vec3(0.945098, 0.913725, 0.749020), Vec3(0.972549, 0.788235, 0.372549), Vec3(1.000000, 0.666667, 0.000000),
    Vec3(0.800000, 0.501961, 0.000000),
    Vec3(0.600000, 0.341176, 0.000000), Vec3(0.415686, 0.203922, 0.011765), Vec3(0.258824, 0.117647, 0.058824),
    Vec3(0.098039, 0.027451, 0.101961),
    Vec3(0.035294, 0.003922, 0.184314), Vec3(0.015686, 0.015686, 0.286275), Vec3(0.000000, 0.027451, 0.392157),
    Vec3(0.047059, 0.172549, 0.541176)
};

double calculate_iterations(const Complex c, const int escape_boundary_squared, const int max_iterations)
{
    double i = 0;
    Complex z = Complex::ZERO;
    while (z.magnitude_squared() < escape_boundary_squared && i < max_iterations)
    {
        z = z.squared() + c;
        i++;
    }

    if (i >= max_iterations) return i;

    // Smoothing
    const double log_zn = std::log(z.magnitude_squared()) / 2.;
    const double log_2 = std::log(2);
    const double nu = std::log(log_zn / log_2) / log_2;
    return i + 1 - nu;
}

Vec3 get_mandelbrot_color(const double iterations, const int max_iterations)
{
    if (std::isnan(iterations) || iterations >= max_iterations) return Vec3::ZERO;

    const int integer = static_cast<int>(std::floor(iterations));
    const double decimal = iterations - static_cast<double>(integer);
    // The smoothing term can push `iterations` below zero, and C++ modulo keeps the sign
    const int index = (integer % PALETTE_SIZE + PALETTE_SIZE) % PALETTE_SIZE;
    const int next_index = (index + 1) % PALETTE_SIZE;
    return PALETTE[index] + (PALETTE[next_index] - PALETTE[index]) * decimal;
}

Complex get_coordinate(const int row, const int col, const MandelbrotSceneSpace& mss)
{
    // Sample pixel centres, so that a 1-pixel-wide or 1-pixel-tall image does not divide by zero
    double re = mss.left + ((static_cast<double>(col) + .5) / static_cast<double>(mss.width_pixels)) * (mss.right - mss.
        left);
    double im = mss.top + ((static_cast<double>(row) + .5) / static_cast<double>(mss.height_pixels)) * (mss.bottom - mss
        .top);
    return {re, im};
}


int main()
{
    std::ifstream i(MANDELBROT_DATA_PATH);
    if (!i)
    {
        std::cerr << "Cannot open " << MANDELBROT_DATA_PATH << '\n';
        return -1;
    }

    MandelbrotSceneConfig scene;
    try
    {
        json j;
        i >> j;
        scene = j;
    }
    catch (const std::exception& e)
    {
        std::cerr << "JSON parsing failed: " << e.what() << '\n';
        return -1;
    }

    if (scene.width <= 0 || scene.aspect_ratio <= 0 || scene.zoom <= 0 || scene.max_iterations <= 0)
    {
        std::cerr << "Invalid scene: width, aspect_ratio, zoom and max_iterations must all be greater than 0.\n";
        return -1;
    }

    const int HEIGHT = static_cast<int>(static_cast<double>(scene.width) / scene.aspect_ratio);
    const double LEFT = -scene.aspect_ratio / scene.zoom + scene.center.x;
    const double RIGHT = scene.aspect_ratio / scene.zoom + scene.center.x;
    const double BOTTOM = -1. / scene.zoom + scene.center.y;
    const double TOP = 1 / scene.zoom + scene.center.y;

    auto img = Image(scene.width, HEIGHT, 256, false);
    const MandelbrotSceneSpace mss(
        LEFT,
        RIGHT,
        BOTTOM,
        TOP,
        HEIGHT,
        scene.width,
        scene.center
    );

    std::cout << "Rendering the mandelbrot set...\n";
    {
        std::atomic finished_rows = 0;
        const int number_of_threads = static_cast<int>(std::max(1u, std::thread::hardware_concurrency()));

        std::jthread progress_thread{
            [&finished_rows, HEIGHT]
            {
                while (true)
                {
                    const int current_row = finished_rows.load();
                    clear_current_stdout_row();
                    display_percentage(current_row, HEIGHT, "row");
                    if (current_row >= HEIGHT) return;
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
                for (int row = thread_idx; row < HEIGHT; row += number_of_threads)
                {
                    for (int col = 0; col < scene.width; col++)
                    {
                        img.draw(row, col,
                                 get_mandelbrot_color(
                                     calculate_iterations(
                                         get_coordinate(row, col, mss),
                                         scene.escape_boundary_squared,
                                         scene.max_iterations
                                     ),
                                     scene.max_iterations
                                 )
                        );
                    }
                    ++finished_rows;
                }
            });
        }
    }
    clear_current_stdout_row();
    display_percentage(HEIGHT, HEIGHT, "row");

    std::cout << "\nRendered!\n\nSaving to " << scene.output_path << "...";
    img.generateBmp(scene.output_path);
    std::cout << "\nSaved!\n";
    return 0;
}
