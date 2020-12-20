#include "examples/coins.h"
#include <algorithm>
#include <gtest/gtest.h>


TEST(CoinsTest, SimpleCoinTest) {
	auto result = ways(5);
	EXPECT_EQ(2, result.size());
	EXPECT_NE(result.end(), std::find(result.begin(), result.end(), std::list<unsigned int>{ 1, 1, 1, 1, 1 }));
	EXPECT_NE(result.end(), std::find(result.begin(), result.end(), std::list<unsigned int>{ 5 }));
}

TEST(CoinsTest, SolveCoinsVariants) {
	auto result = ways(17);
	EXPECT_EQ(6, result.size());
	EXPECT_NE(result.end(), std::find(result.begin(), result.end(), std::list<unsigned int>{ 10, 5, 1, 1 }));
	EXPECT_NE(result.end(), std::find(result.begin(), result.end(), std::list<unsigned int>{ 5, 5, 5, 1, 1 }));
	EXPECT_NE(result.end(), std::find(result.begin(), result.end(), std::list<unsigned int>{ 5, 5, 1, 1, 1, 1, 1, 1, 1 }));
	EXPECT_NE(result.end(),
						std::find(result.begin(), result.end(), std::list<unsigned int>{ 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }));
	EXPECT_NE(
		result.end(),
		std::find(result.begin(), result.end(), std::list<unsigned int>{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }));
	EXPECT_NE(result.end(), std::find(result.begin(), result.end(), std::list<unsigned int>{ 10, 1, 1, 1, 1, 1, 1, 1 }));
}