#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "common/image.h"
#include "common/util/error_handling.h"
#include "raytracer/scene.h"
#include "raytracer/shapes/floor.h"
#include "raytracer/shapes/sphere.h"
#include "common/util/vec3.h"
#include "raytracer/materials/lambertian.h"
#include "raytracer/materials/lambertian_texture.h"
#include "raytracer/materials/metal.h"
#include "raytracer/materials/cubemap.h"
#include "raytracer/camera.h"
#include "raytracer/renderer.h"

namespace raytracer::common
{
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vec3, x, y, z)
} // namespace raytracer::common

namespace raytracer::raytracer
{

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RayTracerSceneConfig, background_color, sky_color, camera, objects,
                                   is_shading_thresholded, thresholded_shading_degrees, is_grayscale, output_path)

int main()
{
    std::vector<std::unique_ptr<Object>> objects;
    objects.emplace_back(std::make_unique<Sphere>(std::make_unique<const Lambertian>(common::Vec3(1, 1, 0)),
                                                  common::Vec3(6, -5, -20), 6));
    objects.emplace_back(std::make_unique<Sphere>(std::make_unique<const Metal>(common::Vec3(0.4, 0.5, 0.6), 0.1),
                                                  common::Vec3(-6, -5, -20), 6));
    objects.emplace_back(
        std::make_unique<Sphere>(std::make_unique<const LambertianTexture>(Cubemap(
                                     common::Image(common::Vec3(1, 0, 0)), common::Image(common::Vec3(0, 1, 0)),
                                     common::Image(common::Vec3(0, 0, 1)), common::Image(common::Vec3(1, 1, 0)),
                                     common::Image(common::Vec3(0, 1, 1)), common::Image(common::Vec3(1, 0, 1)))),
                                 common::Vec3(0, 10, -120), 20));
    objects.emplace_back(std::make_unique<Floor>(std::make_unique<const Metal>(common::Vec3(0.4, 0.4, 0.8), 0.6), -11));

    // Scene config
    const RayTracerSceneConfig scene(common::Vec3(0.39, 0.582, 0.9258), common::Vec3(1, 1, 1),
                                     Camera(-1.0, 1.0, -1.0, 1.0, 1.0, common::Vec3::ZERO, common::Vec3(0, 0, -1),
                                            common::Vec3(0, 1, 0), 800, 800, ViewType::PERSPECTIVE, 256, 4, 50),
                                     std::move(objects), false, 4.0, false, "raytracer.bmp");

    Renderer renderer;
    const auto img = renderer.render(scene, std::cout);
    std::cout << "\nRendered!\n\nSaving to " << scene.output_path << "...";
    img.generateBmp(scene.output_path);
    std::cout << "\nSaved!\n";
    return 0;
}
} // namespace raytracer::raytracer

int main()
{
    try
    {
        return raytracer::raytracer::main();
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
