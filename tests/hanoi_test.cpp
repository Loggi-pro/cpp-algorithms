#include "examples/hanoi.h"
#include <gtest/gtest.h>


TEST(HanoiTest, SolveHanoiTower) {
	HanoiTower A("A");
	HanoiTower B("B");
	HanoiTower C("C");
	A.push(1);
	A.push(2);
	A.push(3);
	solveHanoiPuzzle(A, B, C);
	EXPECT_EQ(0, A.size());
	EXPECT_EQ(3, B.size());
	EXPECT_EQ(0, C.size());
	EXPECT_EQ(3, B.top());
	B.pop();
	EXPECT_EQ(2, B.top());
	B.pop();
	EXPECT_EQ(1, B.top());
}
TEST(HanoiTest, SolveInvalidHanoiTower_ShouldThrowException) {
	HanoiTower A("A");
	HanoiTower B("B");
	HanoiTower C("C");

	EXPECT_THROW(solveHanoiPuzzle(A, B, C), std::runtime_error);
}
