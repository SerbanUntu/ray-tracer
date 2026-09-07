#pragma once
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "common/util/vec3.h"
#include "camera.h"
#include "shapes/object.h"

namespace raytracer::raytracer
{
using json = nlohmann::json;

struct RayTracerSceneConfig
{
    common::Vec3 background_color;
    common::Vec3 sky_color;
    Camera camera;

    std::vector<std::unique_ptr<Object>> objects;

    // Post-processing
    bool is_shading_thresholded;
    double thresholded_shading_degrees;
    bool is_grayscale;

    std::string output_path;
};
} // namespace raytracer::raytracer


