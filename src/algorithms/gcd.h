#pragma once
#include <algorithm>
template<typename T>
inline T gcd(T a, T b) {
	if (a % b == 0) {
		return std::min(a, b);
	} else {
		return gcd<T>(b, a % b);
	}
}
