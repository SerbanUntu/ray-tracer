#include <gtest/gtest.h>

TEST(Test, TestOnePlusOne) {
	const auto expected = 2;
	const auto actual = 1 + 1;
	ASSERT_EQ(expected, actual);
}

TEST(Test, TestTwoPlusTwo) {
	const auto expected = 4;
	const auto actual = 2 + 2;
	ASSERT_EQ(expected, actual);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}