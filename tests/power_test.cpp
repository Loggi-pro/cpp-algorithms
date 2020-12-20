#include "algorithms/power.h"
#include <gtest/gtest.h>


TEST(PowerTest, SimpleTest) {
	EXPECT_EQ(32, power(2, 5));
	EXPECT_EQ(16, power(2, 4));
	EXPECT_EQ(2, power(2, 1));
	EXPECT_EQ(1, power(1, 10));
	EXPECT_EQ(0, power(0, 10));
	EXPECT_EQ(1, power(0, 0));
}
