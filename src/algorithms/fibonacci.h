#pragma once
#include <utility>

namespace details {
inline unsigned int fibonacci(std::pair<unsigned int, unsigned int> v, unsigned int n) {
	return n > 0 ? fibonacci(std::make_pair(v.first + v.second, v.first), n - 1) : v.first;
}
}// namespace details


inline unsigned int fibonacci(unsigned int n) {
	return n == 0 ? 0 : details::fibonacci(std::make_pair(1, 0), n - 1);
}
