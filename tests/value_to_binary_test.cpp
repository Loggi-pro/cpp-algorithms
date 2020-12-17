#include "examples/value_to_binary.h"
#include <gtest/gtest.h>


TEST(ValueToBinary, ConvertZero) {
	unsigned int a = 0;
	EXPECT_EQ(std::string{ "0" }, atobin(a));
}
TEST(ValueToBinary, ConvertValue) {
	unsigned int a = 254;
	EXPECT_EQ(std::string{ "11111110" }, atobin(a));
}