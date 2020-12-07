#include <gtest/gtest.h>

int Factorial(int n) {
	int result = 1;
	for (int i = 1; i <= n; i++) { result *= i; }

	return result;
}

class FactorialTest : public ::testing::Test {
public:
	void SetUp() override {}

	void TearDown() override {}

protected:
	/* none yet */
};

TEST_F(FactorialTest, Negative) {
	EXPECT_EQ(1, Factorial(-5));
	EXPECT_EQ(1, Factorial(-1));
	EXPECT_GT(Factorial(-10), 0);
}

// Tests factorial of 0.
TEST_F(FactorialTest, Zero) {
	EXPECT_EQ(1, Factorial(0));
}