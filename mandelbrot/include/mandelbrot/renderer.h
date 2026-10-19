#pragma once
#include <thread>
#include <span>

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
    /**
     * Render the Mandelbrot set to an image given a configuration.
     *
     * @param scene The object containing the details of the scene.
     * @param out The std::ostream to which to write rendering progress updates (defaults to none).
     * @param number_of_threads The number of parallel threads used for rendering.
     * @return The rendered image.
     */
    static common::Image render(
        const MandelbrotSceneConfig &scene, std::ostream &out = common::get_nullout(),
        int number_of_threads = static_cast<int>(std::max(1U, std::thread::hardware_concurrency())));
};
} // namespace raytracer::mandelbrot
