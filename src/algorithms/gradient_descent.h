#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
template<std::size_t N>
std::array<double, N> calc_derivatives_nominal(std::array<double, N> &xs, double (*f)(std::array<double, N>)) {
	const double dh = 1.0 / 1000000.0;
	const double fn = f(xs);
	std::array<double, N> result{ 0 };
	double sum = 0;
	for (std::size_t i = 0; i < N; ++i) {
		xs[i] += dh;
		double dfdx = (f(xs) - fn) / dh;
		result[i] = dfdx;
		sum += result[i] * result[i];
		xs[i] -= dh;
	}
	double length = std::sqrt(sum);
	for (double &v : result) { v /= length; }
	return result;
}


template<std::size_t N>
std::array<double, N>
	gradient_search(std::array<double, N> inits, std::array<double, N> hs, double (*f)(std::array<double, N>)) {
	const double TOLERANCE = 1.0 / 100000.0;
	std::array<double, N> xs{ inits };
	double yprev = 0;
	double y = f(xs);

	while (std::abs(y - yprev) > TOLERANCE) {
		auto s = calc_derivatives_nominal<N>(xs, f);
		for (std::size_t i = 0; i < N; ++i) { xs[i] = xs[i] - hs[i] * s[i]; }
		yprev = y;
		y = f(xs);
		if (y > yprev) {
			for (double &a : hs) { a /= 2; }
		}
	}
	return xs;
}