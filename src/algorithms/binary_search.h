#pragma once

template<class T>
T *binary_search(T *first, T *last, const T &value) {
	if (first > last) { return nullptr; }

	T *middle = first + (last - first) / 2;
	if (*middle > value) {
		return binary_search<T>(first, middle - 1, value);
	} else if (*middle < value) {
		return binary_search<T>(middle + 1, last, value);
	}

	return middle;
}