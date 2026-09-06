#pragma once
#include <nlohmann/json.hpp>

namespace raytracer::common
{
struct Point
{
    double x;
    double y;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y);
} // namespace raytracer::common
