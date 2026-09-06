#include <random>
#include "common/util/random_utils.h"

static thread_local std::mt19937 gen{std::random_device{}()};
static thread_local std::uniform_real_distribution dist(-1.0, 1.0);

Vec3 random_unit()
{
    return Vec3(dist(gen), dist(gen), dist(gen)).to_normalized();
}

std::mt19937& get_generator() { return gen; }