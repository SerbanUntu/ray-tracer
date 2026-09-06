#include <util/vec3.h>
#include <gtest/gtest.h>

TEST(Vec3Test, CrossProductSimple)
{
    constexpr Vec3 up{0, 1, 0};
    constexpr Vec3 forward{0, 0, -1};
    constexpr Vec3 left{-1, 0, 0};
    EXPECT_EQ(up, forward.cross(left));
}

TEST(Vec3Test, CrossProductComplex)
{
    constexpr Vec3 one{1, 2, 3};
    constexpr Vec3 two{4, 5, 6};
    constexpr Vec3 three{-3, 6, -3};
    EXPECT_EQ(three, one.cross(two));
}
