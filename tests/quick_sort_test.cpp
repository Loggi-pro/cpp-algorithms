#include "algorithms/quick_sort.h"
#include <gtest/gtest.h>
#include <iterator>
TEST(QuickSort, SortArray) {
	int arr[] = { 9, 7, 5, 3, 1, 0 };
	quick_sort(std::begin(arr), std::end(arr));
	EXPECT_EQ(0, arr[0]);
	EXPECT_EQ(1, arr[1]);
	EXPECT_EQ(3, arr[2]);
	EXPECT_EQ(5, arr[3]);
	EXPECT_EQ(7, arr[4]);
	EXPECT_EQ(9, arr[5]);
}

TEST(QuickSort, SortEmptyArray) {
	int arr[] = { 0 };
	quick_sort(std::end(arr), std::end(arr));
	EXPECT_EQ(0, arr[0]);
}