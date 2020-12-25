#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
namespace details {

template<typename It, typename Comparator>
It partition(It first, It last, Comparator cmp) {
	It left = first;
	It right = last;
	It pivot = left++;
	while (left != right) {
		if (cmp(*left, *pivot)) {
			++left;
		} else {
			while ((left != right) && cmp(*pivot, *right)) { --right; }
			std::iter_swap(left, right);
		}
	}

	--left;
	std::iter_swap(pivot, left);
	return left;
}
}// namespace details
template<typename It, typename Comparator>
void quick_sort(It first, It last, Comparator cmp) {
	if (first == last) { return; }
	It pivot = details::partition(first, last, cmp);
	quick_sort(first, pivot++, cmp);
	quick_sort(pivot, last, cmp);
}

template<typename It>
inline void quick_sort(It first, It last) {
	quick_sort(first, last, std::less_equal<typename std::iterator_traits<It>::value_type>{});
}