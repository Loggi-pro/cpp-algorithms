#include "algorithms/insertion_sort.h"
#include <gtest/gtest.h>
#include <utils.h>


TEST(InsertionSortTest, VectorSorted) {
	std::vector<int> v{ 32, 45, 59, 80, 91, 46 };
	insertion_sort<int>(v.begin(), v.end());
	std::vector<int> expected{ 32, 45, 46, 59, 80, 91 };
	for (auto val : zip(expected, v)) { EXPECT_EQ(val.first, val.second); }
}

TEST(InsertionSortTest, EmptyVector_ShouldNotThrowException) {
	std::vector<int> v{};
	insertion_sort<int>(v.begin(), v.end());
}
