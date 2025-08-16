#include <util/vec3.h>
#include <gtest/gtest.h>

TEST(Vec3Test, CrossProductSimple) {
	const Vec3 up = Vec3(0, 1, 0);
	const Vec3 forward = Vec3(0, 0, -1);
	const Vec3 left = Vec3(-1, 0, 0);
	ASSERT_EQ(up, forward.cross(left));
}

TEST(Vec3Test, CrossProductComplex) {
	const Vec3 one = Vec3(1, 2, 3);
	const Vec3 two = Vec3(4, 5, 6);
	const Vec3 three = Vec3(-3, 6, -3);
	ASSERT_EQ(three, one.cross(two));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}