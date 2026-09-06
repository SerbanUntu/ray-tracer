#pragma once
#include <nlohmann/json.hpp>
#include "common/util/point.h"

using json = nlohmann::json;

struct MandelbrotSceneConfig
{
    Point center;
    int width;
    double aspect_ratio;
    double zoom;
    int max_iterations;
    int escape_boundary_squared;
    std::string output_path;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    MandelbrotSceneConfig,
    center,
    width,
    aspect_ratio,
    zoom,
    max_iterations,
    escape_boundary_squared,
    output_path
);

struct MandelbrotSceneSpace
{
    double left;
    double right;
    double bottom;
    double top;
    int height_pixels;
    int width_pixels;
    Point center;
};
