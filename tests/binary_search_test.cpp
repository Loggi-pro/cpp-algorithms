#include "algorithms/binary_search.h"
#include <gtest/gtest.h>

TEST(BinarySearchTest, SearchExistElement_ShouldReturnPointer) {
	int arr[] = { 0, 1, 3, 5, 7, 9 };
	EXPECT_EQ(0, binary_search(arr, std::end(arr), 0) - arr);
	EXPECT_EQ(1, binary_search(arr, std::end(arr), 1) - arr);
	EXPECT_EQ(2, binary_search(arr, std::end(arr), 3) - arr);
	EXPECT_EQ(3, binary_search(arr, std::end(arr), 5) - arr);
	EXPECT_EQ(4, binary_search(arr, std::end(arr), 7) - arr);
	EXPECT_EQ(5, binary_search(arr, std::end(arr), 9) - arr);
}

TEST(BinarySearchTest, SearchNotExistElement_ShouldReturnNullptr) {
	int arr[] = { 0, 1, 3, 5, 7, 9 };
	EXPECT_EQ(nullptr, binary_search(arr, std::end(arr), 2));
}