#pragma once
#include <thread>

#include "scene.h"
#include "common/image.h"
#include "common/util/complex.h"
#include "common/util/terminal.h"

namespace raytracer::mandelbrot
{
class Renderer
{
    static double calculate_iterations(common::Complex c, int escape_boundary_squared, int max_iterations);
    static common::Vec3 get_mandelbrot_color(double iterations, int max_iterations,
                                             std::span<const common::Vec3> palette);
    static common::Complex get_coordinate(int row, int col, const MandelbrotSceneSpace &mss);

  public:
    static common::Image render(
        const MandelbrotSceneConfig &scene, std::ostream &out = common::get_nullout(),
        int number_of_threads = static_cast<int>(std::max(1U, std::thread::hardware_concurrency())));
};
} // namespace raytracer::mandelbrot
