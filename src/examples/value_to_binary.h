#pragma once
#include <algorithm>
#include <string>
namespace Private {
std::string atobin(unsigned int a, std::string s, int index) {
	if (a == 0 || a == 1) {
		s.insert(0, 1, static_cast<char>(a + '0'));
		return s;
	}

	char res = static_cast<char>(a % 2 + '0');
	s.insert(0, 1, res);
	return atobin(a / 2, std::move(s), index + 1);
}
}// namespace Private

std::string atobin(unsigned int a) {
	std::string result;
	result = Private::atobin(a, std::move(result), 0);
	return result;
}
