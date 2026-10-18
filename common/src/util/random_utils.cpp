#include <random>
#include "common/util/random_utils.h"

namespace raytracer::common
{
static thread_local std::uniform_real_distribution dist(-1.0, 1.0);

Vec3 random_unit()
{
    return Vec3(dist(get_generator()), dist(get_generator()), dist(get_generator())).to_normalized();
}

std::mt19937 &get_generator()
{
    thread_local std::mt19937 gen{std::random_device{}()};
    return gen;
}
} // namespace raytracer::common
