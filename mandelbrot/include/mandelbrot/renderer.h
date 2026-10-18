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
    double calculate_iterations(common::Complex c, int escape_boundary_squared, int max_iterations);
    common::Vec3 get_mandelbrot_color(double iterations, int max_iterations, std::span<const common::Vec3> palette);
    common::Complex get_coordinate(int row, int col, const MandelbrotSceneSpace &mss);

  public:
    common::Image render(const MandelbrotSceneConfig &scene, std::ostream &out = common::nullout,
                         int number_of_threads = static_cast<int>(std::max(1U, std::thread::hardware_concurrency())));
};
} // namespace raytracer::mandelbrot
