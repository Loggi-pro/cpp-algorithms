#include "algorithms/gcd.h"
#include <gtest/gtest.h>


TEST(GcdTest, SimpleTest) {
	EXPECT_EQ(7, gcd(35, 21));
	EXPECT_EQ(1, gcd(8, 15));
	EXPECT_EQ(1, gcd(1, 1));
	EXPECT_EQ(0, gcd(0, 1));
}
