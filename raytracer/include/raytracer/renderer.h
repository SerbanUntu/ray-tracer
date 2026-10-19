#pragma once
#include <thread>

#include "scene.h"
#include "common/image.h"
#include "common/util/terminal.h"
#include "common/util/vec3.h"

namespace raytracer::raytracer
{
class Renderer
{
    common::Vec3 shade(const Object &o, const common::Vec3 &intersection, const Ray &r, int depth, int max_depth,
                       const RayTracerSceneConfig &scene);
    common::Vec3 trace(const Ray &r, int depth, int max_depth, const RayTracerSceneConfig &scene);

  public:
    /**
     * Render the 3D scene to an image given a configuration.
     *
     * @param scene The object containing the details of the scene (objects, etc.).
     * @param out The std::ostream to which to write rendering progress updates (defaults to none).
     * @param number_of_threads The number of parallel threads used for rendering.
     * @return The rendered image.
     */
    common::Image render(const RayTracerSceneConfig &scene, std::ostream &out = common::get_nullout(),
                         int number_of_threads = static_cast<int>(std::max(1U, std::thread::hardware_concurrency())));
};
} // namespace raytracer::raytracer
