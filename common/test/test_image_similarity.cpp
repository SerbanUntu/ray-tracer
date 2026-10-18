#include "common/image_similarity.h"
#include <cmath>
#include <stdexcept>
#include <gtest/gtest.h>

namespace raytracer::common
{
static constexpr double EPSILON = 1e-12;
static const double SQRT_3 = std::sqrt(3.0);

static Image make_uniform_image(const int width, const int height, const Vec3 &color)
{
    Image img(width, height, 256, false);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            img.draw(row, col, color);
        }
    }
    return img;
}

static Image make_gradient_image(const int width, const int height)
{
    Image img(width, height, 256, false);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            img.draw(row, col, Vec3(static_cast<double>(row) / height, static_cast<double>(col) / width, 0.25));
        }
    }
    return img;
}

static Image make_checkerboard_image(const int width, const int height)
{
    Image img(width, height, 256, false);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            img.draw(row, col, (row + col) % 2 == 0 ? Vec3(1, 1, 1) : Vec3::ZERO);
        }
    }
    return img;
}

TEST(ImageSimilarityTest, ThrowsOnDifferentWidth)
{
    const Image a = make_uniform_image(4, 4, Vec3::ZERO);
    const Image b = make_uniform_image(5, 4, Vec3::ZERO);
    EXPECT_THROW(diff_images(a, b), std::invalid_argument);
}

TEST(ImageSimilarityTest, ThrowsOnDifferentHeight)
{
    const Image a = make_uniform_image(4, 4, Vec3::ZERO);
    const Image b = make_uniform_image(4, 5, Vec3::ZERO);
    EXPECT_THROW(diff_images(a, b), std::invalid_argument);
}

TEST(ImageSimilarityTest, ThrowsOnZeroDownsamplingFactor)
{
    const Image a = make_uniform_image(4, 4, Vec3::ZERO);
    EXPECT_THROW(diff_images(a, a, 0), std::invalid_argument);
}

TEST(ImageSimilarityTest, IdenticalImagesHaveZeroDiff)
{
    const Image a = make_gradient_image(17, 9);
    for (const unsigned short factor : {1, 2, 4, 8})
        EXPECT_DOUBLE_EQ(0.0, diff_images(a, a, factor)) << "factor " << factor;
}

TEST(ImageSimilarityTest, IdenticalBlackImagesHaveZeroDiff)
{
    const Image a = make_uniform_image(8, 8, Vec3::ZERO);
    EXPECT_DOUBLE_EQ(0.0, diff_images(a, a, 4));
}

TEST(ImageSimilarityTest, BlackVsWhiteIsMaximal)
{
    const Image black = make_uniform_image(8, 8, Vec3::ZERO);
    const Image white = make_uniform_image(8, 8, Vec3(1, 1, 1));
    for (const unsigned short factor : {1, 2, 4})
        EXPECT_NEAR(SQRT_3, diff_images(black, white, factor), EPSILON) << "factor " << factor;
}

TEST(ImageSimilarityTest, BrightnessDifferenceIsDetected)
{
    const Image gray = make_uniform_image(8, 8, Vec3(0.5, 0.5, 0.5));
    const Image white = make_uniform_image(8, 8, Vec3(1, 1, 1));
    EXPECT_NEAR(SQRT_3 / 2.0, diff_images(gray, white, 4), EPSILON);
}

TEST(ImageSimilarityTest, IsSymmetric)
{
    const Image a = make_gradient_image(10, 6);
    const Image b = make_checkerboard_image(10, 6);
    EXPECT_DOUBLE_EQ(diff_images(a, b, 3), diff_images(b, a, 3));
}

TEST(ImageSimilarityTest, FactorOneIsPerPixelMeanDistance)
{
    const Image a = make_uniform_image(4, 4, Vec3::ZERO);
    Image b = make_uniform_image(4, 4, Vec3::ZERO);
    b.draw(1, 2, Vec3(1, 0, 0));
    EXPECT_NEAR(1.0 / 16.0, diff_images(a, b, 1), EPSILON);
}

TEST(ImageSimilarityTest, SinglePixelDifferenceInLastBlockIsDetected)
{
    const Image a = make_uniform_image(8, 8, Vec3::ZERO);
    Image b = make_uniform_image(8, 8, Vec3::ZERO);
    b.draw(7, 7, Vec3(1, 0, 0));
    for (const unsigned short factor : {1, 2, 4, 8})
        EXPECT_NEAR(1.0 / 64.0, diff_images(a, b, factor), EPSILON) << "factor " << factor;
}

TEST(ImageSimilarityTest, PartialBlocksAreWeightedByArea)
{
    const Image a = make_uniform_image(5, 3, Vec3::ZERO);
    Image b = make_uniform_image(5, 3, Vec3::ZERO);
    b.draw(2, 4, Vec3(1, 0, 0));
    EXPECT_NEAR(1.0 / 15.0, diff_images(a, b, 2), EPSILON);
}

TEST(ImageSimilarityTest, FactorLargerThanImageComparesGlobalAverage)
{
    const Image a = make_uniform_image(3, 3, Vec3::ZERO);
    Image b = make_uniform_image(3, 3, Vec3::ZERO);
    b.draw(0, 0, Vec3(0, 0, 1));
    EXPECT_NEAR(1.0 / 9.0, diff_images(a, b, 8), EPSILON);
}

TEST(ImageSimilarityTest, DownsamplingAveragesOutHighFrequencyNoise)
{
    const Image checkerboard = make_checkerboard_image(8, 8);
    const Image gray = make_uniform_image(8, 8, Vec3(0.5, 0.5, 0.5));
    EXPECT_NEAR(SQRT_3 / 2.0, diff_images(checkerboard, gray, 1), EPSILON);
    EXPECT_NEAR(0.0, diff_images(checkerboard, gray, 2), EPSILON);
}
} // namespace raytracer::common
