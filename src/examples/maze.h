#pragma once

class Position {
protected:
	int _row = 0;
	int _col = 0;

public:
	Position() = default;
	Position(int row, int col) : _row(row), _col(col) {}
	void setPosition(int row, int col) {
		_row = row;
		_col = col;
	}
	int getRow() const {
		return _row;
	}
	int getColumn() const {
		return _col;
	}
};


class PositionIterator {
protected:
	struct itrFields {
		int row, column, direction;
	};
	itrFields field;

public:
	// Init
	PositionIterator() = default;
	~PositionIterator() {}
	// init with pos Position
	PositionIterator(const Position &pos) {
		field.row = pos.getRow();
		field.column = pos.getColumn();
		field.direction = 0;
	}
	Position operator++(int) {
		int nextRow = field.row;
		int nextColumn = field.column;

		switch (field.direction++) {
		case 0:// north;
			nextRow = field.row - 1;
			break;

		case 1:// east
			nextColumn = field.column + 1;
			break;

		case 2:// south
			nextRow = field.row + 1;
			break;

		case 3:// west
			nextColumn = field.column - 1;
			break;
		}

		Position next(nextRow, nextColumn);
		return next;
	}
	// true-> end
	bool atEnd() {
		return field.direction >= 3;
	}
};


class Application {
public:
	Position virtual generateInitialState() = 0;
	// true->pos on the way to target
	// false->not on the way
	bool virtual valid(const Position &pos) const = 0;
	// save pos
	void virtual record(const Position &pos) = 0;
	// true if pos is final position
	bool virtual done(const Position &pos) const = 0;
	// pos is not on the way to target
	void virtual undo(const Position &pos) = 0;
	Application() {}
	virtual ~Application() {}
};


class BackTrack {
public:
	BackTrack(Application *app) : _app(app) {}
	// True if solution is fonded
	bool tryToSolve(const Position &pos) {
		PositionIterator it = PositionIterator(pos);
		bool success = false;

		while (!success && !it.atEnd()) {
			Position newPos = it++;

			if (!_app->valid(newPos)) { continue; }

			//допустимая позиция
			_app->record(newPos);

			if (_app->done(newPos)) {
				success = true;
			} else {
				success = tryToSolve(newPos);
				if (!success) { _app->undo(newPos); }
			}// undo
		}

		return success;
	}

protected:
	Application *_app;
};


#include <set>
class MazeApplication : public Application {
	Position start;
	Position finish;
	enum { WALL = 0, CORRIDOR = 1, PATH = 9, TRIED = 2 };
	inline static const int ROWS = 7;
	inline static const int COLUMNS = 13;
	inline static int grid[ROWS][COLUMNS] = {
		{ 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1 }, { 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 }, { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

public:
	std::set<std::pair<int, int>> positions;
	Position generateInitialState() {
		start.setPosition(0, 0);
		finish.setPosition(6, 12);
		record(start);
		return start;
	}
	// Postcondition:	true->pos on the way to target
	//				false->not on the way
	bool valid(const Position& pos) const {
		return (pos.getRow() >= 0 && pos.getRow() < ROWS && pos.getColumn() >= 0 && pos.getColumn() < COLUMNS
						&& grid[pos.getRow()][pos.getColumn()] == CORRIDOR);
	}
	// pos must be legal
	void record(const Position& pos) {
		grid[pos.getRow()][pos.getColumn()] = PATH;
		positions.insert(std::make_pair(pos.getColumn(), pos.getRow()));
	}
	// true - when pos is final position
	bool done(const Position &pos) const {
		return pos.getRow() == finish.getRow() && pos.getColumn() == finish.getColumn();
	}
	// pos will be checked as a position not on the way
	void undo(const Position &pos) {
		grid[pos.getRow()][pos.getColumn()] = TRIED;
		positions.extract(std::make_pair(pos.getColumn(), pos.getRow()));
	}
	MazeApplication() {
		finish.setPosition(6, 12);
	}
};
