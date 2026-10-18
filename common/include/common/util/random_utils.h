#pragma once
#include <random>
#include "vec3.h"

namespace raytracer::common
{
constexpr unsigned TEST_SEED = 123;

bool set_seed(unsigned new_seed);
bool remove_seed();
Vec3 random_unit();
std::mt19937 &get_generator();
} // namespace raytracer::common
