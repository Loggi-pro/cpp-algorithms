#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>

#include "iterators/enumerate.h"

// Generate maze: Euler

struct MazeCellInfo {
	bool isRightWallExist = false;
	bool isBottomWallExist = false;
	friend std::ostream& operator<<(std::ostream& s, const MazeCellInfo& cell);
};

std::ostream& operator<<(std::ostream& s, const MazeCellInfo& cell) {
	std::string v;
	if (cell.isRightWallExist && cell.isBottomWallExist) {
		s << "_|";
	} else if (cell.isBottomWallExist) {
		s << "__";
	} else if (cell.isRightWallExist) {
		s << " |";
	} else {
		s << "  ";
	}
	return s;
}


template<std::size_t cols>
struct MazeRow {
	std::array<MazeCellInfo, cols> array;
};


template<std::size_t rows, std::size_t cols>
struct Maze {
	std::array<MazeRow<cols>, rows> array;
	[[nodiscard]] std::size_t colCount() const {
		return cols;
	}
	[[nodiscard]] std::size_t rowCount() const {
		return rows;
	}
	template<std::size_t rs, std::size_t cs>
	friend std::ostream& operator<<(std::ostream& s, const Maze<rs, cs>& maze);
};

template<std::size_t rows, std::size_t cols>
std::ostream& operator<<(std::ostream& s, const Maze<rows, cols>& maze) {
	for (std::size_t j = 0; j < maze.colCount(); ++j) { s << "__"; }
	s << std::endl;
	for (std::size_t i = 0; i < maze.rowCount(); ++i) {
		s << "|";
		for (std::size_t j = 0; j < maze.colCount(); ++j) {
			const auto& cell = maze.array[i].array[j];
			s << cell;
		}
		s << std::endl;
	}
	return s;
}


template<std::size_t cols>
struct MazeRowGenerator {
	std::function<bool()> randomBoolGenerator;
	int setCounter_ = 1;
	int c = 0;
	std::array<int, cols> setInfo;
	explicit MazeRowGenerator(unsigned int seed)
		: randomBoolGenerator(std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine(seed))) {
		// Step 1
		randomBoolGenerator = [&]() { return c++ % 2; };
		nullifyRow();
	}

	void reset() {
		setCounter_ = 1;
		nullifyRow();
	}

	MazeRow<cols> generateRow() {
		MazeRow<cols> row;
		// Step 2
		assignUniqueSet();
		// Step 3
		addingVerticalWalls(row);
		// Step 4
		addingHorizontalWalls(row);
		checkedHorizontalWalls(row);
		// Step 5.1
		preparatingNewLine(row);
		return row;
	}

	MazeRow<cols> generateEndRow() {
		MazeRow<cols> row;
		assignUniqueSet();
		addingVerticalWalls(row);
		checkedEndLine(row);
		return row;
	}

private:
	void nullifyRow() {
		for (auto& set : setInfo) { set = 0; }
	}

	void assignUniqueSet() {
		for (auto& set : setInfo) {
			if (set != 0) { continue; }
			set = setCounter_++;
		}
	}

	void addingVerticalWalls(MazeRow<cols>& row) {
		for (std::size_t i = 0; i < cols - 1; i++) {
			const bool isMakeWall = randomBoolGenerator();
			// Проверка условия для предотовращения зацикливания
			if (isMakeWall || setInfo[i] == setInfo[i + 1]) {
				row.array[i].isRightWallExist = true;
			} else {
				_mergeSet(i, std::size_t(i + 1));
			}
		}
		// Добавление правой стенки в последней ячейки
		row.array[cols - 1].isRightWallExist = true;
	}

	void addingHorizontalWalls(MazeRow<cols>& row) {
		for (auto i : enumerate(row.array)) {
			// Ставим стенку или нет
			const bool isMakeWall = randomBoolGenerator();
			// Проверка, что множество имеет более одной ячейки (это предовратит замкнутые области )
			if (isMakeWall && _calculateUniqueSet(setInfo[i]) != 1) {
				// Ставим горизонтальную стенку
				row.array[i].isBottomWallExist = true;
			}
		}
	}


	// Проверка условие 4.1 и 4.2
	void checkedHorizontalWalls(MazeRow<cols>& row) {
		for (auto i : enumerate(setInfo)) {
			if (_calculateHorizontalWalls(row, setInfo[i]) == 0) { row.array[i].isBottomWallExist = false; }
		}
	}

	// Подсчет горизонтальных стен у ячеек уникального множества
	int _calculateHorizontalWalls(const MazeRow<cols>& row, int element) const {
		int countHorizontalWalls = 0;
		for (auto i : enumerate(setInfo)) {
			if (setInfo[i] == element && !row.array[i].isBottomWallExist) { countHorizontalWalls++; }
		}
		return countHorizontalWalls;
	}

	void preparatingNewLine(MazeRow<cols>& row) {
		for (std::size_t i : enumerate(row.array)) {
			if (row.array[i].isBottomWallExist) {
				// Присваиваем ячейки пустое множество
				setInfo[i] = 0;
			}
		}
	}

	void checkedEndLine(MazeRow<cols>& row) {
		for (std::size_t i = 0; i < cols - 1; i++) {
			// Проверка условия пункта 5.2.1
			if (setInfo[i] != setInfo[i + 1]) {
				// Убираем вертикальную стенку
				row.array[i].isRightWallExist = false;
				// Объединяем множества
				_mergeSet(i, i + 1);
			}
			// Добавляем горизонтальные стенки
			row.array[i].isBottomWallExist = true;
		}
		// Добавляем горизонтальную стенку в последней ячейке
		row.array[cols - 1].isBottomWallExist = true;
	}

	// Объединение ячеек в одно множество
	void _mergeSet(std::size_t index, std::size_t index2) {
		const int sourceSet = setInfo[index];
		const int mutableSet = setInfo[index2];
		for (std::size_t i = index + 1; i < cols; i++) {
			//Проверка ячеек на одно множество
			if (setInfo[i] == mutableSet) {
				// Объединение ячейки в множество
				setInfo[i] = sourceSet;
			}
		}
	}

	// Подсчет ячеек, которые содержаться в уникальном множестве
	[[nodiscard]] int _calculateUniqueSet(int element) const {
		int countUniqSet = 0;
		for (std::size_t i : enumerate(setInfo)) {
			if (setInfo[i] == element) { countUniqSet++; }
		}
		return countUniqSet;
	}
};


template<std::size_t rows, std::size_t cols>
inline Maze<rows, cols> generateMaze(unsigned int seed) {
	Maze<rows, cols> maze;
	MazeRowGenerator<cols> generator{ seed };
	for (std::size_t irow = 0; irow < rows - 1; irow++) { maze.array[irow] = generator.generateRow(); }
	// Step 5.2
	maze.array[rows - 1] = generator.generateEndRow();
	return maze;
}
