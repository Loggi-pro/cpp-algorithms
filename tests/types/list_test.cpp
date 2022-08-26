#include <gtest/gtest.h>

#include "types/list.h"
#include <string>

TEST(ListTest, TestPushBack) {
	List<std::string> l;
	std::vector<std::string> expected{ "cat", "dog", "dog", "duck", "lion", "monkey" };
	for (const auto& s : expected) { l.push_back(s); }
	EXPECT_EQ(expected.size(), l.size());
	unsigned int i = 0;
	for (auto& s : l) {
		EXPECT_EQ(expected[i], s);
		i++;
	}
}
TEST(ListTest, TestPop) {
	List<std::string> l;
	std::vector<std::string> expected{ "cat", "dog", "dog", "duck", "lion", "monkey" };
	for (const auto& s : expected) { l.push_back(s); }

	EXPECT_EQ(expected.size(), l.size());
	for (std::size_t i = 0; i < expected.size(); ++i) {
		std::size_t index = expected.size() - 1 - i;
		EXPECT_EQ(expected[index], l.back());
		l.pop_back();
		EXPECT_EQ(expected.size() - 1 - i, l.size());
	}
}