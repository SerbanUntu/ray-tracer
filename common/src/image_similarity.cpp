#include "common/image_similarity.h"
#include <stdexcept>
#include <algorithm>

namespace raytracer::common
{
double diff_images(const Image &img1, const Image &img2, const unsigned short downsampling_factor)
{
    if (downsampling_factor == 0)
    {
        throw std::invalid_argument("The downsampling factor must be greater than 0");
    }
    if (img1.get_height() != img2.get_height())
    {
        throw std::invalid_argument("Cannot diff two images that have a different height");
    }
    if (img1.get_width() != img2.get_width())
    {
        throw std::invalid_argument("Cannot diff two images that have a different width");
    }

    double sum_of_differences = 0.0;
    const auto height = img1.get_height();
    const auto width = img1.get_width();
    for (int block_row = 0; block_row <= (height - 1) / downsampling_factor; ++block_row)
    {
        for (int block_col = 0; block_col <= (width - 1) / downsampling_factor; ++block_col)
        {
            const int remaining_rows =
                std::min(static_cast<int>(downsampling_factor), height - block_row * downsampling_factor);
            const int remaining_cols =
                std::min(static_cast<int>(downsampling_factor), width - block_col * downsampling_factor);
            Vec3 v1 = Vec3::ZERO;
            Vec3 v2 = Vec3::ZERO;

            for (int i = 0; i < remaining_rows; ++i)
            {
                for (int j = 0; j < remaining_cols; ++j)
                {
                    v1 += img1.get_color(block_row * downsampling_factor + i, block_col * downsampling_factor + j);
                    v2 += img2.get_color(block_row * downsampling_factor + i, block_col * downsampling_factor + j);
                }
            }

            sum_of_differences += (v1 - v2).length();
        }
    }
    return sum_of_differences / (width * height);
}
} // namespace raytracer::common
