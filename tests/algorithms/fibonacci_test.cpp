#include "algorithms/fibonacci.h"
#include <gtest/gtest.h>
#include <vector>


TEST(FibonacciTest, TestZero) {
	EXPECT_EQ(0, fibonacci(0));
}
TEST(FibonacciTest, TestFirst) {
	EXPECT_EQ(1, fibonacci(1));
}

TEST(FibonacciTest, TestSequence) {
	std::vector<int> expected{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144 };
	for (unsigned int n = 0; n < expected.size(); ++n) {
		unsigned int f = fibonacci(n);
		EXPECT_EQ(expected[n], f);
	}
}

TEST(FibonacciTest, TestMatrix) {
	unsigned int a = fibbonacci_matrix<unsigned int>(25u);
	EXPECT_EQ(75025u, a);
}