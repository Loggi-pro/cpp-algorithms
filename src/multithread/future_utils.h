#pragma once
#include <future>

template<typename T>
auto split(std::future<T>& x) {
	auto tmp = std::move(x);
	std::promise<T> p;
	x = p.get_future();// replace x with new future
	return tmp.then([_p = std::move(p)](auto _tmp) mutable {
		if (_tmp.has_exception()) {
			auto error = _tmp.get_exception_ptr();
			_p.set_excepton(error);
			std::rethrow_exception(error);
		}

		auto value = _tmp.get();
		_p.set_value(value);// assign to new x future
		return value;
	});
}