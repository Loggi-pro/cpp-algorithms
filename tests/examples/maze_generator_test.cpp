#include <gtest/gtest.h>

#include "examples/maze_generator.h"

TEST(MazeGeneratorTest, SimpleMaze) {
	(void)generateMaze<4, 4>(0);
	// std::cout << maze;
}