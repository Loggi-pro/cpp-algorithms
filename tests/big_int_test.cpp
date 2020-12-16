#include "types/big_int.h"
#include <gtest/gtest.h>


TEST(BigIntTest, BigInt_SummSmallNumbers) {
	bigint_t t1("999999");
	bigint_t t("12345");
	bigint_t t2 = t + t1;
	EXPECT_EQ(999999 + 12345, t2.value().value());
}

TEST(BigIntTest, BigInt_SummBigNumbers_ShouldReturnNone) {
	auto max = std::numeric_limits<unsigned int>::max();
	bigint_t t1(max);
	bigint_t t(1);
	bigint_t t2 = t + t1;
	EXPECT_EQ(false, t2.value().has_value());
	std::string value = to_string(t2);
	EXPECT_EQ("4294967296", to_string(t2));
}
