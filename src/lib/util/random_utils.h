#pragma once
#include <random>
#include "vec3.h"

Vec3 random_unit();
std::mt19937& get_generator();