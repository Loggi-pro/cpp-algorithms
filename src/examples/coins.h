#pragma once
#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

// Coin type
struct Penny {
	const unsigned int value = 1;
};
struct Fivecent {
	const unsigned int value = 5;
};
struct Tencent {
	const unsigned int value = 10;
};
struct Twentyfive {
	const unsigned int value = 25;
};
using Coin = std::variant<std::monostate, Penny, Fivecent, Tencent, Twentyfive>;

// overload std::visit
template<class... Ts>
struct overloaded : Ts... {
	using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;// not needed as of c++20


// method for Coin to take value
inline auto value(Coin coin) {
	return std::visit(overloaded{ [&](const auto &c) { return c.value; },
																[&](const std::monostate & /*unused*/) -> unsigned int { return 0; } },
										coin);
}

namespace utils {
// util to generate tuple from index sequence
template<std::size_t I>
using index_t = std::integral_constant<std::size_t, I>;
template<std::size_t I>
inline constexpr index_t<I> index_expander{};

template<std::size_t... Is>
constexpr std::tuple<index_t<Is>...> make_indexes(std::index_sequence<Is...>) {
	return std::make_tuple(index_expander<Is>...);
}
template<std::size_t N>
inline constexpr auto indexing_tuple = make_indexes(std::make_index_sequence<N>{});


template<std::size_t... Is>
constexpr auto index_sequence_reverse(std::index_sequence<Is...> const &)
	-> decltype(std::index_sequence<sizeof...(Is) - 1U - Is...>{});

template<std::size_t N>
using make_index_sequence_reverse = decltype(index_sequence_reverse(std::make_index_sequence<N>{}));

// util to iterate over tuple ( or index sequence)
template<std::size_t... Is, class F, class T>
auto tuple_foreach(std::index_sequence<Is...>, F &&f, T &&tup) {
	(f(std::get<Is>(std::forward<T>(tup))), ...);
}

// iterate forward
template<class F, class T>
auto tuple_foreach(F &&f, T &&tup) {
	const std::size_t size = std::tuple_size<std::decay_t<T>>::value;
	auto indexes = std::make_index_sequence<size>{};
	return tuple_foreach(indexes, std::forward<F>(f), std::forward<T>(tup));
}

// iterate reverse order
template<class F, class T>
auto tuple_foreach_reverse(F &&f, T &&tup) {
	const std::size_t size = std::tuple_size<std::decay_t<T>>::value;
	auto indexes = make_index_sequence_reverse<size>{};
	return tuple_foreach(indexes, std::forward<F>(f), std::forward<T>(tup));
}
}// namespace utils

namespace details {
// main backtrack algorithm N - count of variant types
template<int N>
int ways(std::vector<std::list<unsigned int>> &s, int amount) {
	// size of variant=1(only monostate exist
	if (amount < 0 || N == 1) { return 0; }
	if (amount == 0) {
		auto last = s.back();
		s.emplace_back(std::list<unsigned int>{ last.begin(), last.end() });
		return 1;
	}

	int sum = 0;
	bool break_flag = false;
	auto indexes = utils::indexing_tuple<N>;
	utils::tuple_foreach_reverse(
		[&](auto I) {// I is compile time integer
			if (!break_flag) {
				const int index = static_cast<int>(I);
				const int index_prev = static_cast<int>(I) - 1;
				const unsigned int val = value(std::variant_alternative_t<I, Coin>{});
				if (val <= static_cast<unsigned int>(amount)) {
					s.back().push_back(val);
					sum += ways<index + 1>(s, amount - static_cast<int>(val));
					s.back().pop_back();
					sum += ways<index_prev + 1>(s, amount);
					break_flag = true;
				}
			}
		},
		indexes);
	return sum;
}
}// namespace details
// call function
inline std::vector<std::list<unsigned int>> ways(int amount) {
	std::vector<std::list<unsigned int>> result;
	result.emplace_back(std::list<unsigned int>{});
	details::ways<std::variant_size_v<Coin>>(result, amount);
	result.pop_back();
	return result;
}


// for debug only
/*
std::ostream &operator<<(std::ostream &out, std::list<unsigned int> const &data) {
	for (unsigned int a : data) { std::cout << a << ","; }

	std::cout << std::endl;
	return out;
}
*/