#pragma once
#include <nlohmann/json.hpp>

struct Point
{
    double x;
    double y;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y);
