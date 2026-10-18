#include <random>
#include "common/util/random_utils.h"

namespace raytracer::common
{
static bool is_seed_set = false;
static bool is_seed_frozen = false;
static unsigned seed;
static thread_local std::uniform_real_distribution dist(-1.0, 1.0);

bool set_seed(const unsigned new_seed)
{
    if (is_seed_frozen)
        return false;
    seed = new_seed;
    is_seed_set = true;
    return true;
}

bool remove_seed()
{
    if (is_seed_frozen)
        return false;
    is_seed_set = false;
    return true;
}

Vec3 random_unit()
{
    auto &gen = get_generator();
    return Vec3(dist(gen), dist(gen), dist(gen)).to_normalized();
}

std::mt19937 &get_generator()
{
    thread_local std::mt19937 gen{is_seed_set ? seed : std::random_device{}()};
    is_seed_frozen = true;
    return gen;
}
} // namespace raytracer::common
