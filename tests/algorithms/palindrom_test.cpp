#include "algorithms/palindrom.h"
#include <gtest/gtest.h>


TEST(PalindromTest, SimpleTest) {
	EXPECT_EQ(false, isPalindrom("EV"));
	EXPECT_EQ(true, isPalindrom("ABADABA"));
	EXPECT_EQ(true, isPalindrom("RADAR"));
	EXPECT_EQ(true, isPalindrom("OTTO"));
	EXPECT_EQ(true, isPalindrom("MADAMIMADAM"));
	EXPECT_EQ(true, isPalindrom("EVE"));
	EXPECT_EQ(true, isPalindrom("Madam, i'm Adam"));
	EXPECT_EQ(true, isPalindrom("Able was I'ere I saw Elba"));
	EXPECT_EQ(true, isPalindrom("A man. A plan. A canal. Panama!"));
}