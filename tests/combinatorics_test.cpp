#include "algorithms/combinatorics.h"
#include <gtest/gtest.h>


TEST(CombinatoricsTest, SimpleTest) {
	std::string s{ "AB" };
	EXPECT_EQ(true, start_permutation(s.begin(), s.end()));
	EXPECT_EQ("BA", s);
	EXPECT_EQ(false, start_permutation(s.begin(), s.end()));
}