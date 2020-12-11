#pragma once
#include <algorithm>
namespace Private {
template<class T, class RandomAccessIterator>
void _unguarded_linear_insert(RandomAccessIterator last, T value) {
	auto next = last;
	--next;
	while (value < *next) {
		*last = *next;
		last	= next--;
	}
	*last = value;
}

template<class T, class RandomAccessIterator>
inline void _linear_insert(RandomAccessIterator first, RandomAccessIterator last) {
	T value = *last;
	if (value < *first) {
		std::copy_backward(first, last, last + 1);
		*first = value;
	} else {
		_unguarded_linear_insert(last, value);
	}
}
}// namespace Private
//Сортировка вставками
template<class T, class RandomAccessIterator>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (first == last) return;
	for (auto i = first + 1; i != last; ++i) { Private::_linear_insert<T>(first, i); }
}
