#include <random>
#include "random_utils.h"

std::uniform_real_distribution<> dist(-1., 1.);

Vec3 random_unit() {
	return Vec3(dist(gen), dist(gen), dist(gen)).to_normalized();
}
