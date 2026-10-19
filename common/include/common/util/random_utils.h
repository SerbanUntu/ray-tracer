#pragma once
#include <random>
#include "vec3.h"

namespace raytracer::common
{
constexpr unsigned TEST_SEED = 123;

/**
 * Sets the global RNG seed to the new value, if it is not frozen.
 *
 * @param new_seed The new global seed.
 * @return True if the generator has not been created yet (the seed is not frozen), false otherwise.
 */
bool set_seed(unsigned new_seed);

/**
 * Resets the global seed so the generator will use a random device instead, if the seed is not frozen.
 *
 * @return True if the generator has not been created yet (the seed is not frozen), false otherwise.
 */
bool remove_seed();

/**
 * Returns a random 3D vector of length 1.
 *
 * @return A random 3D vector of length 1.
 */
Vec3 random_unit();

/**
 * Returns a reference to the thread-local RNG.
 *
 * @return A reference to the thread-local RNG.
 */
std::mt19937 &get_generator();
} // namespace raytracer::common
