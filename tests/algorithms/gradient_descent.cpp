#include "algorithms/gradient_descent.h"
#include <gtest/gtest.h>
TEST(GradientDescentTest, FindMinOn2dGraphic) {
	auto res = gradient_search<1>(std::array<double, 1>{ 0 }, std::array<double, 1>{ 0.5 }, [](std::array<double, 1> a) {
		return 19.0794 - 16.1368 * a[0] + 4.0897 * a[0] * a[0];
	});
	EXPECT_NEAR(1.97265625, res[0], 1.0 / 1000);
}

TEST(GradientDescentTest, FindMinOn3dGraphic) {
	auto res = gradient_search<2>(std::array<double, 2>{ 1, 1 },
																std::array<double, 2>{ 0.5, 0.5 },
																[](std::array<double, 2> a) { return std::pow(a[0], 2) + 25 * pow(a[1], 2); });
	EXPECT_NEAR(0, res[0], 1.0 / 1000);
	EXPECT_NEAR(0, res[1], 1.0 / 1000);
}
