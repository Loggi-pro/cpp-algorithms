#pragma once
#include "power.h"
#include <array>
#include <concepts>
#include <utility>

namespace details {

inline unsigned int fibonacci(std::pair<unsigned int, unsigned int> v, unsigned int n) {
	return n > 0 ? fibonacci(std::make_pair(v.first + v.second, v.first), n - 1) : v.first;
}

template<class T>
concept Integral = std::is_integral<T>::value;
template<class T>
concept SignedIntegral = Integral<T>&& std::is_signed<T>::value;
template<class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

template<typename N>
struct multiply_2x2 {
	std::array<N, 4> operator()(const std::array<N, 4>& x, const std::array<N, 4>& y) {
		return { x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3], x[2] * y[0] + x[3] * y[2], x[2] * y[1] + x[3] * y[3] };
	}
};

template<typename N>
std::array<N, 4> identity_element(const multiply_2x2<N>&) {
	return { N(1), N(0), N(0), N(1) };
}
}// namespace details

template<typename R>
auto fibbonacci_matrix(details::UnsignedIntegral auto n) {
	if (n == 0) { return R(0); }

	return power(std::array<R, 4>{ 1, 1, 1, 0 }, static_cast<decltype(n)>(n - 1), details::multiply_2x2<R>())[0];
}

inline unsigned int fibonacci(unsigned int n) {
	return n == 0 ? 0 : details::fibonacci(std::make_pair(1, 0), n - 1);
}
