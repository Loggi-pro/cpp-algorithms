#include "sort/insertion_sort.h"
#include <gtest/gtest.h>


template<typename T1, typename T2>
std::vector<std::pair<T1, T2>> zip(const std::vector<T1> &v1, const std::vector<T2> &v2) {
	std::vector<std::pair<int, int>> res(v1.size());
	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(res), [](const auto &aa, const auto &bb) {
		return std::pair{ aa, bb };
	});
	return res;
}

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
