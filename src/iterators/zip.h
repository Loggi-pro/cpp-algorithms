#pragma once
#include <algorithm>
#include <utility>
#include <vector>

#include <cassert>
#include <cmath>
#include <functional>
#include <list>
#include <string>
#include <typeinfo>

template<typename T1, typename T2>
std::vector<std::pair<T1, T2>> zip(const std::vector<T1>& v1, const std::vector<T2>& v2) {
	std::vector<std::pair<int, int>> res(v1.size());
	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(res), [](const auto& aa, const auto& bb) {
		return std::pair{ aa, bb };
	});
	return res;
}

namespace details {
template<typename Iter>
using select_access_type_for =
	std::conditional_t<std::is_same_v<Iter, std::vector<bool>::iterator> || std::is_same_v<Iter, std::vector<bool>::const_iterator>,
										 typename Iter::value_type,
										 typename Iter::reference>;


template<typename... Args, std::size_t... Index>
auto any_match_impl(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs, std::index_sequence<Index...>)
	-> bool {
	auto result = false;
	result = (... | (std::get<Index>(lhs) == std::get<Index>(rhs)));
	return result;
}

template<typename... Args>
auto any_match(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs) -> bool {
	return any_match_impl(lhs, rhs, std::index_sequence_for<Args...>{});
}

template<typename... Iters>
class zip_iterator {
public:
	using value_type = std::tuple<select_access_type_for<Iters>...>;

	zip_iterator() = delete;

	zip_iterator(Iters&&... iters) : m_iters{ std::forward<Iters>(iters)... } {}

	auto operator++() -> zip_iterator& {
		std::apply([](auto&&... args) { ((args += 1), ...); }, m_iters);
		return *this;
	}

	auto operator++(int) -> zip_iterator {
		auto tmp = *this;
		++*this;
		return tmp;
	}

	auto operator!=(zip_iterator const& other) {
		return !(*this == other);
	}

	auto operator==(zip_iterator const& other) {
		return any_match(m_iters, other.m_iters);
	}

	auto operator*() -> value_type {
		return std::apply([](auto&&... args) { return value_type(*args...); }, m_iters);
	}

private:
	std::tuple<Iters...> m_iters;
};


/* std::decay needed because T is a reference, and is not a complete type */
template<typename T>
using select_iterator_for = std::conditional_t<std::is_const_v<std::remove_reference_t<T>>,
																							 typename std::decay_t<T>::const_iterator,
																							 typename std::decay_t<T>::iterator>;


template<typename... T>
class zipper {
public:
	using zip_type = zip_iterator<select_iterator_for<T>...>;

	template<typename... Args>
	zipper(Args&&... args) : m_args{ std::forward<Args>(args)... } {}

	auto begin() -> zip_type {
		return std::apply([](auto&&... args) { return zip_type(std::begin(args)...); }, m_args);
	}
	auto end() -> zip_type {
		return std::apply([](auto&&... args) { return zip_type(std::end(args)...); }, m_args);
	}

private:
	std::tuple<T...> m_args;
};
}// namespace details

template<typename... T>
auto zip(T&&... t) {
	return details::zipper<T...>{ std::forward<T>(t)... };
}
