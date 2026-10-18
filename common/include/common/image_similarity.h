#pragma once
#include "image.h"

namespace raytracer::common
{
double diff_images(const Image &img1, const Image &img2, unsigned short downsampling_factor = 4);
} // namespace raytracer::common