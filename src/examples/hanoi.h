#pragma once
#include <iostream>
#include <stack>
#include <string>
class HanoiTower {
private:
	std::stack<int> _stack;
	std::string _name;
	std::ostream *_outstream;

public:
	HanoiTower(std::string name, std::ostream *s = nullptr) : _stack{}, _name(std::move(name)), _outstream(s) {}
	std::string getName() const {
		return _name;
	}
	void pop() {
		if (_outstream) *_outstream << "move disk " << _stack.top() << " from " << this << std::endl;
		_stack.pop();
	}
	void push(int disk) {
		if (_outstream) *_outstream << "move disk " << disk << " to " << this << std::endl;
		_stack.push(disk);
	}
	std::size_t size() const {
		return _stack.size();
	}
	const int &top() const {
		return _stack.top();
	}
};


std::ostream &operator<<(std::ostream &out, HanoiTower const &data) {
	out << data.getName();
	// and so on...
	return out;
}


inline void solveHanoiPuzzle(HanoiTower &origin, HanoiTower &dest, HanoiTower &temp) {
	if (origin.size() == 1) {
		int disk = origin.top();
		origin.pop();
		dest.push(disk);
	} else if (origin.size() > 1) {
		int disk = origin.top();
		origin.pop();
		solveHanoiPuzzle(origin, temp, dest);
		solveHanoiPuzzle(temp, dest, origin);
		dest.push(disk);
	} else {
		throw std::runtime_error("Puzzle can't be solved.");
	}
}
