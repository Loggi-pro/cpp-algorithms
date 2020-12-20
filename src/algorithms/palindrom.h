#pragma once
#include <cctype>
#include <string>
#include <utility>
inline bool isPalindrom(std::string s, std::size_t i, std::size_t j) {
	while (i < j) {
		if (!std::isalpha(s[i])) { i++; }
		if (!std::isalpha(s[j])) { j--; }
		if (std::isalpha(s[i]) && std::isalpha(s[j])) { break; }
	}

	int si = std::tolower(s[i]);
	int sj = std::tolower(s[j]);

	if (i >= j) { return si == sj; }
	return si == sj && isPalindrom(std::move(s), i + 1, j - 1);
}

inline bool isPalindrom(std::string s) {
	return isPalindrom(s, 0, s.length() - 1);
}