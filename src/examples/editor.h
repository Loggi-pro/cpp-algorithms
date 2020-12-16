#pragma once
#include "types/list.h"
#include <iterator>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
class TextEditor {
	enum Command { None, Insert, Delete, Line, Done, Last, Print, Change };
	List<std::string> _text;
	List<std::string>::iterator _current{ _text.begin() };
	Command _cmd{ None };

	int _currentLineNumber{ -1 };
	struct Argument {
		struct DeleteArg {
			int from;
			int to;
		};
		struct LineArg {
			int to;
		};
		struct ChangeArg {
			std::string from;
			std::string to;
		};
		struct PrintArg {
			int from;
			int to;
		};
		PrintArg printArg{};
		ChangeArg changeArg;
		LineArg lineArg{};
		DeleteArg delArg{};

		Argument() = default;
		explicit Argument(DeleteArg args) : delArg(std::move(args)) {}
		explicit Argument(LineArg args) : lineArg(std::move(args)) {}
		explicit Argument(ChangeArg args) : changeArg(std::move(args)) {}
		explicit Argument(PrintArg args) : printArg(std::move(args)) {}
		~Argument() = default;
	};

	std::tuple<bool, Command, Argument> _isCorrectCommand(const std::string &line) {
		std::istringstream buf(line);
		std::istream_iterator<std::string> beg(buf);
		std::istream_iterator<std::string> end;
		std::vector<std::string> tokens(beg, end);

		if (tokens.empty()) { return std::make_tuple<bool, Command, Argument>(false, None, Argument()); }

		std::string &cmd = tokens[0];

		if (cmd == "Insert") {
			if (tokens.size() != 1) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Insert, Argument());
		} else if (cmd == "Delete") {
			if (tokens.size() != 3) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Delete, Argument(Argument::DeleteArg{ std::stoi(tokens[1]), std::stoi(tokens[2]) }));
		} else if (cmd == "Line") {
			if (tokens.size() != 2) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Line, Argument(Argument::LineArg{ std::stoi(tokens[1]) }));
		} else if (cmd == "Done") {
			if (tokens.size() != 1) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Done, Argument());
		} else if (cmd == "Last") {
			if (tokens.size() != 1) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Last, Argument());
		} else if (cmd == "Change") {
			if (tokens.size() != 3) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Change, Argument(Argument::ChangeArg{ tokens[1], tokens[2] }));
		} else if (cmd == "Print") {
			if (tokens.size() != 3) { return std::make_tuple(false, None, Argument()); }

			return std::make_tuple(true, Print, Argument(Argument::PrintArg{ std::stoi(tokens[1]), std::stoi(tokens[2]) }));
		} else {
			return std::make_tuple(false, None, Argument());
		}
	}
	void _commandDelete(int from, int to) {
		if (from < 0) { throw std::runtime_error("To small index to delete"); }

		if (to >= (int)_text.size()) { throw std::runtime_error("Too big index to delete"); }

		if (from > to) { throw std::runtime_error("First index bigger than second"); }

		List<std::string>::iterator it = _text.begin();

		for (int i = 0; i < from; i++) { it++; }

		for (int i = from; i <= to; i++) {
			_text.erase(it);
			it++;
		}

		if (_currentLineNumber >= from && _currentLineNumber <= to) {
			_currentLineNumber = from - 1;
			_current = --it;
		} else if (_currentLineNumber > to) {
			_currentLineNumber -= to + 1 - from;// current неизменно
		}

		_cmd = Delete;
	}
	std::string _commandLast() {
		_cmd = Last;
		std::stringstream ss;
		ss << _currentLineNumber;
		return ss.str();
	}
	std::optional<std::unique_ptr<List<std::string>>> _executeCommand(Command cmd, const Argument &arg) {
		switch (cmd) {
		default:
		case None:
			return {};
		case Insert:
			_commandInsertFirstPart();
			break;

		case Delete:
			_commandDelete(arg.delArg.from, arg.delArg.to);
			break;

		case Line:
			_commandLine(arg.lineArg.to);
			break;

		case Done:
			return std::make_optional<std::unique_ptr<List<std::string>>>(_commandDone());
			break;

		case Last: {
			std::unique_ptr<List<std::string>> list = std::make_unique<List<std::string>>();
			list->push_back(_commandLast());
			return std::make_optional<std::unique_ptr<List<std::string>>>(std::move(list));
		}

		case Change:
			_commandChange(arg.changeArg.from, arg.changeArg.to);
			break;

		case Print:
			return std::make_optional<std::unique_ptr<List<std::string>>>(_commandPrint(arg.printArg.from, arg.printArg.to));
			break;
		}

		return {};
	}
	void _commandInsertFirstPart() {
		_cmd = Insert;
	}
	void _commandInsertSecondPart(std::string line) {
		_current = _text.insert(++_current, line);
		_currentLineNumber++;
	}
	void _commandLine(int n) {
		if (n < -1) { throw std::runtime_error("Line argument too small"); }

		if (n >= (int)_text.size()) { throw std::runtime_error("Line argument too large"); }

		if (_currentLineNumber < n) {
			for (int i = _currentLineNumber; i < n; ++i) { _current++; }
		} else {
			for (int i = _currentLineNumber; i > n; --i) { _current--; }
		}

		_currentLineNumber = n;
		_cmd = Line;
	}
	std::unique_ptr<List<std::string>> _commandDone() {

		auto res = std::make_unique<List<std::string>>();
		for (auto &s : _text) { res->push_back(s); }
		_cmd = Done;
		return res;
	}
	void _commandChange(std::string from, std::string to) {
		//_cmd = Change;
		(void)from;
		(void)to;
	}

	std::unique_ptr<List<std::string>> _commandPrint(int from, int to) {
		if (from < 0) { throw std::runtime_error("To small index to print"); }

		if (to >= (int)_text.size()) { throw std::runtime_error("Too big index to print"); }

		if (from > to) { throw std::runtime_error("First index bigger than second"); }

		auto res = std::make_unique<List<std::string>>();
		List<std::string>::iterator it = _text.begin();

		for (int i = 0; i < from; i++) { continue; }

		for (int i = from; i <= to; i++) {
			it++;
			std::string s;

			if (_currentLineNumber == i) { s += '>'; }

			s += (*it);
			res->push_back(s);
		}

		_cmd = Print;
		return res;
	}

public:
	TextEditor() : _current(_text.begin()) {}
	std::optional<std::unique_ptr<List<std::string>>> parse(std::string line) {
		const char *COMMAND_START = "$";

		if (line.substr(0, 1) == COMMAND_START) {
			auto [isCmd, cmd, arg] = _isCorrectCommand(line.substr(1));

			if (!isCmd) { throw std::runtime_error("Not an command"); }

			return _executeCommand(cmd, arg);
		}
		if (_cmd != Insert) { throw std::runtime_error("Not in insert command mode"); }

		// insert string;
		_commandInsertSecondPart(line);
		return {};
	}
	[[nodiscard]] bool isDone() const {
		return _cmd == Done;
	}
	[[nodiscard]] const List<std::string> &text() const {
		return _text;
	}
};