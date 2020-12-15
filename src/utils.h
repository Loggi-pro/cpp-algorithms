#pragma once
#include <algorithm>
#include <utility>
#include <vector>
template<typename T1, typename T2>
std::vector<std::pair<T1, T2>> zip(const std::vector<T1> &v1, const std::vector<T2> &v2) {
	std::vector<std::pair<int, int>> res(v1.size());
	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(res), [](const auto &aa, const auto &bb) {
		return std::pair{ aa, bb };
	});
	return res;
}