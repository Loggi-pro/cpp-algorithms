#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// only for positive numbers
struct bigint_t {
private:
	friend class digitIterator_const;
	using value_type = uint16_t;
	enum { BASE = 10000 };
	std::vector<value_type> _digits;

	// TODO make iterate over every digit, not place(5 digits)
	class place_iterator_const : std::iterator<std::forward_iterator_tag, bigint_t::value_type> {
	private:
		const bigint_t &_obj;
		bool flag_range_out;
		std::size_t _index;

	public:
		place_iterator_const(const bigint_t &o, std::size_t pos)
			: _obj(o), flag_range_out(pos >= o._digits.size()), _index(flag_range_out ? 0 : o._digits.size() - 1 - pos) {
			uint32_t a = 0;
			a++;
		}
		value_type operator*() const {
			return flag_range_out ? 0 : _obj._digits[_index];
		}
		value_type operator->() const {
			return flag_range_out ? 0 : _obj._digits[_index];
		}
		place_iterator_const &operator++() {
			flag_range_out = _index == 0;
			if (_index > 0) { _index--; }
			return *this;
		}


		[[nodiscard]] place_iterator_const operator++(int) {
			place_iterator_const temp{ *this };
			flag_range_out = _index == 0;
			if (_index > 0) { _index--; }

			return temp;
		}
		bool operator==(const place_iterator_const &other) const {
			return _index == other._index && flag_range_out == other.flag_range_out;
		}
		bool operator!=(const place_iterator_const &other) const {
			return !(_index == other._index && flag_range_out == other.flag_range_out);
		}
	};

	[[nodiscard]] place_iterator_const begin_place() const {
		return place_iterator_const{ *this, 0 };
	}
	[[nodiscard]] place_iterator_const end_place() const {
		return place_iterator_const{ *this, _digits.size() };
	}

	void _fromString(std::string str) {
		unsigned int sum = 0;
		value_type carry = 0;
		unsigned int pos = 1;

		for (unsigned int index = 0; index < str.length(); index++) {
			std::size_t i = str.length() - 1 - index;
			sum += static_cast<unsigned char>(str[i] - '0') * pos + carry;
			pos *= 10;
			carry = 0;

			if (sum > BASE) {
				pos = 1;
				_digits.push_back(static_cast<value_type>(sum % BASE));
				carry = static_cast<value_type>(sum / BASE);
				sum = 0;

				if (carry > 0) { pos *= 10; }
			}
		}

		if (sum > 0) { _digits.push_back(static_cast<value_type>(sum)); }

		if (carry > 0) { _digits.push_back(carry); }

		std::reverse(_digits.begin(), _digits.end());
		/*std::transform(str.begin(), str.end_place(), digits.begin_place(), [](const auto & c)-> char {
			return c - '30';
		});*/
	}

public:
	friend std::ostream &operator<<(std::ostream &s, const bigint_t &val);
	explicit bigint_t(std::string str) {
		_fromString(std::forward<std::string>(str));
	}
	bigint_t(const bigint_t &other) = default;

	explicit bigint_t(unsigned int a) {
		_fromString(std::forward<std::string>(std::to_string(a)));
	}

	bigint_t() = default;


	bigint_t operator+(const bigint_t &other) {
		bigint_t sum;
		const bigint_t *bigger = _digits.size() > other._digits.size() ? this : &other;
		const bigint_t *lesser = bigger == &other ? this : &other;
		sum._digits.resize(bigger->_digits.size());

		auto c1b = bigger->begin_place();
		auto c2b = lesser->begin_place();
		auto c1e = bigger->end_place();
		auto c2e = lesser->end_place();
		value_type carry = 0;
		for (std::size_t i = 0; i < bigger->_digits.size(); i++) {
			value_type c1 = c1b == c1e ? 0 : *c1b++;
			value_type c2 = c2b == c2e ? 0 : *c2b++;
			auto val = static_cast<unsigned int>(c1 + c2 + carry);
			auto a = static_cast<value_type>(val % BASE);
			carry = static_cast<value_type>(val / BASE);
			sum._digits[i] = a;
		}

		if (carry > 0) { sum._digits.push_back(carry); };

		std::reverse(sum._digits.begin(), sum._digits.end());

		return sum;
	}

	[[nodiscard]] std::optional<unsigned int> value() const {
		unsigned int sum = 0;
		unsigned int base = 1;
		for (unsigned int index = 0; index < _digits.size(); index++, base *= BASE) {
			unsigned int old_sum = sum;
			sum += static_cast<value_type>(_digits[_digits.size() - 1 - index]) * (base);
			if (sum < old_sum) { return std::nullopt; }
		}
		return std::optional<unsigned int>{ sum };
	}

	[[nodiscard]] std::size_t size() const {
		auto res = to_string(*this);
		return res.length();
	}

	friend std::string to_string(const bigint_t &self) {
		std::stringstream s;
		s << self;
		return s.str();
	}
};

std::ostream &operator<<(std::ostream &s, const bigint_t &val) {
	for (const auto &a : val._digits) { s << static_cast<bigint_t::value_type>(a); }

	return s;
}