#include "examples/maze.h"
#include <gtest/gtest.h>


TEST(MazeTest, SimpleSolveMaze) {
	MazeApplication app;
	BackTrack b(&app);
	Position start = app.generateInitialState();
	EXPECT_EQ(true, b.tryToSolve(start));
	std::vector<std::pair<int, int>> v(app.positions.begin(), app.positions.end());
	EXPECT_EQ(0, v[0].first);
	EXPECT_EQ(0, v[0].second);
	EXPECT_EQ(1, v[1].first);
	EXPECT_EQ(0, v[1].second);
	EXPECT_EQ(2, v[2].first);
	EXPECT_EQ(0, v[2].second);
	EXPECT_EQ(2, v[3].first);
	EXPECT_EQ(1, v[3].second);
	EXPECT_EQ(3, v[4].first);
	EXPECT_EQ(1, v[4].second);
	EXPECT_EQ(4, v[5].first);
	EXPECT_EQ(1, v[5].second);
	EXPECT_EQ(4, v[6].first);
	EXPECT_EQ(2, v[6].second);
	EXPECT_EQ(4, v[7].first);
	EXPECT_EQ(3, v[7].second);
	EXPECT_EQ(4, v[8].first);
	EXPECT_EQ(4, v[8].second);
	EXPECT_EQ(4, v[9].first);
	EXPECT_EQ(5, v[9].second);
	EXPECT_EQ(4, v[10].first);
	EXPECT_EQ(6, v[10].second);
	EXPECT_EQ(5, v[11].first);
	EXPECT_EQ(6, v[11].second);
	EXPECT_EQ(6, v[12].first);
	EXPECT_EQ(6, v[12].second);
	EXPECT_EQ(7, v[13].first);
	EXPECT_EQ(6, v[13].second);
	EXPECT_EQ(8, v[14].first);
	EXPECT_EQ(6, v[14].second);
	EXPECT_EQ(9, v[15].first);
	EXPECT_EQ(6, v[15].second);
	EXPECT_EQ(10, v[16].first);
	EXPECT_EQ(6, v[16].second);
	EXPECT_EQ(11, v[17].first);
	EXPECT_EQ(6, v[17].second);
	EXPECT_EQ(12, v[18].first);
	EXPECT_EQ(6, v[18].second);
	// EXPECT_EQ()
}