#pragma once
#include "vec3.h"
#include <random>

Vec3 random_unit();

static std::random_device rd;
static std::mt19937 gen(rd());
