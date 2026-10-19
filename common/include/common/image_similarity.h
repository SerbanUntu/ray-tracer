#pragma once
#include "image.h"

namespace raytracer::common
{
/**
 * Computes the average distance between the color vectors of each pixel in two images.
 *
 * @throws std::invalid_argument If the images don't have the same width and height, or if the factor is zero.
 *
 * @param img1 The first image.
 * @param img2 The second image.
 * @param downsampling_factor How many pixels a side of the box has. Four means a 4x4 box filter is applied (colors are
 * averaged over each 4x4 area).
 * @return A value between 0 and sqrt(3), equal to the average distance between the color vectors of each pixel in the
 * two images.
 */
double diff_images(const Image &img1, const Image &img2, unsigned short downsampling_factor = 4);
} // namespace raytracer::common