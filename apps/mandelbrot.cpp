#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <array>
#include <nlohmann/json.hpp>
#include "common/image.h"
#include "mandelbrot/scene.h"
#include "common/util/error_handling.h"
#include "mandelbrot/renderer.h"

namespace raytracer::mandelbrot
{
using namespace std::chrono;

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
    catch (const std::exception &e)
    {
        std::cerr << "JSON parsing failed: " << e.what() << '\n';
        return -1;
    }

    if (scene.width <= 0 || scene.aspect_ratio <= 0 || scene.zoom <= 0 || scene.max_iterations <= 0)
    {
        std::cerr << "Invalid scene: width, aspect_ratio, zoom and max_iterations must all be greater than 0.\n";
        return -1;
    }

    Renderer renderer;
    const auto img = renderer.render(scene, std::cout);
    std::cout << "\nRendered!\n\nSaving to " << scene.output_path << "...";
    img.generateBmp(scene.output_path);
    std::cout << "\nSaved!\n";
    return 0;
}
} // namespace raytracer::mandelbrot

int main()
{
    try
    {
        return raytracer::mandelbrot::main();
    }
    catch (const std::exception &err)
    {
        raytracer::common::reportException(err);
        return 1;
    }
    catch (...)
    {
        raytracer::common::reportUnknownException();
        return 1;
    }
}
