#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include "image.h"
#include "util/complex.h"
#include "util/terminal.h"
#include "scene.h"

using namespace std::chrono;

constexpr auto STDOUT_REFRESH_INTERVAL_MS = 100;
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
    if (iterations >= max_iterations) return Vec3::ZERO;

    const int integer = static_cast<int>(std::floor(iterations));
    const double decimal = iterations - static_cast<double>(integer);
    return PALETTE[integer % PALETTE_SIZE] +
        (PALETTE[(integer + 1) % PALETTE_SIZE] - PALETTE[integer % PALETTE_SIZE]) * decimal;
}

Complex get_coordinate(const int row, const int col, const MandelbrotSceneSpace& mss)
{
    double re = mss.left + (static_cast<double>(col) / (static_cast<double>(mss.width_pixels) - 1.)) * (mss.right - mss.left);
    double im = mss.top + (static_cast<double>(row) / (static_cast<double>(mss.height_pixels) - 1.)) * (mss.bottom - mss.top);
    return {re, im};
}


int main()
{
    std::ifstream i(MANDELBROT_DATA_PATH);

    json j;
    i >> j;
    MandelbrotSceneConfig scene;

    try
    {
        scene = j;
    }
    catch (const std::exception& e)
    {
        std::cerr << "JSON parsing failed: " << e.what() << '\n';
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

    auto start = high_resolution_clock::now();

    for (int row = 0; row < HEIGHT; row++)
    {
        auto now = high_resolution_clock::now();
        const auto dur = duration_cast<milliseconds>(now - start);

        if (row == 0 || row == HEIGHT - 1 || dur.count() > STDOUT_REFRESH_INTERVAL_MS)
        {
            clear_current_stdout_row();
            display_percentage(row + 1, HEIGHT, "row");
            start = now;
        }

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
    }

    std::cout << "\nRendered!\n\nSaving to " << scene.output_path << "...";
    img.generateBmp(scene.output_path);
    std::cout << "\nSaved!\n";
    return 0;
}
