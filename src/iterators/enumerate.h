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

namespace traits {

template<typename C>
struct has_size {
	template<typename T>
	static auto check(T*) -> typename std::is_integral<decltype(std::declval<T const>().size())>::type;

	template<typename>
	static auto check(...) -> std::false_type;

	using type = decltype(check<C>(0));
	static constexpr bool value = type::value;
};

}// namespace traits


namespace details {

template<typename T>
struct range_iter_base : std::iterator<std::input_iterator_tag, T> {
	range_iter_base(T _current) : current(_current) {}

	T operator*() const {
		return current;
	}

	T const* operator->() const {
		return &current;
	}

	range_iter_base& operator++() {
		++current;
		return *this;
	}

	range_iter_base operator++(int) {
		auto copy = *this;
		++*this;
		return copy;
	}

	bool operator==(range_iter_base const& other) const {
		return current == other.current;
	}

	bool operator!=(range_iter_base const& other) const {
		return not(*this == other);
	}

protected:
	T current;
};

template<typename T>
struct step_range_proxy {
	struct iterator : details::range_iter_base<T> {
		iterator(T _current, T step) : details::range_iter_base<T>(_current), step_(step) {}

		using details::range_iter_base<T>::current;

		iterator& operator++() {
			current += step_;
			return *this;
		}

		iterator operator++(int) {
			auto copy = *this;
			++*this;
			return copy;
		}

		// Loses commutativity. Iterator-based ranges are simply broken. :-(
		bool operator==(iterator const& other) const {
			return step_ > 0 ? current >= other.current : current < other.current;
		}

		bool operator!=(iterator const& other) const {
			return not(*this == other);
		}

		T step_;
	};

	step_range_proxy(T begin, T end, T step) : begin_(begin, step), end_(end, step) {}

	iterator begin() const {
		return begin_;
	}

	iterator end() const {
		return end_;
	}

	std::size_t size() const {
		if (*end_ >= *begin_) {
			// Increasing and empty range
			if (begin_.step_ < T{ 0 }) return 0;
		} else {
			// Decreasing range
			if (begin_.step_ > T{ 0 }) return 0;
		}
		return std::ceil(std::abs(static_cast<double>(*end_ - *begin_) / begin_.step_));
	}

private:
	iterator begin_;
	iterator end_;
};

template<typename T>
struct range_proxy {
	struct iterator : details::range_iter_base<T> {
		iterator(T _current) : details::range_iter_base<T>(_current) {}
	};

	range_proxy(T begin, T end) : begin_(begin), end_(end) {}

	step_range_proxy<T> step(T step) {
		return { *begin_, *end_, step };
	}

	iterator begin() const {
		return begin_;
	}

	iterator end() const {
		return end_;
	}

	std::size_t size() const {
		return *end_ - *begin_;
	}

private:
	iterator begin_;
	iterator end_;
};

}// namespace details


template<typename C, typename = typename std::enable_if<traits::has_size<C>::value>>
auto enumerate(C const& cont) -> details::range_proxy<decltype(cont.size())> {
	return { 0, cont.size() };
}

template<typename T, std::size_t N>
details::range_proxy<std::size_t> enumerate(T (&)[N]) {
	return { 0, N };
}

template<typename T>
details::range_proxy<typename std::initializer_list<T>::size_type> enumerate(std::initializer_list<T>&& cont) {
	return { 0, cont.size() };
}
