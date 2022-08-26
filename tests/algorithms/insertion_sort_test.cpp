#include <gtest/gtest.h>

#include "algorithms/insertion_sort.h"
#include "iterators/zip.h"


TEST(InsertionSortTest, VectorSorted) {
	std::vector<int> v{ 32, 45, 59, 80, 91, 46 };
	insertion_sort<int>(v.begin(), v.end());
	std::vector<int> expected{ 32, 45, 46, 59, 80, 91 };
	for (auto [v1, v2] : zip(expected, v)) { EXPECT_EQ(v1, v2); }
}

TEST(InsertionSortTest, EmptyVector_ShouldNotThrowException) {
	std::vector<int> v{};
	insertion_sort<int>(v.begin(), v.end());
}
