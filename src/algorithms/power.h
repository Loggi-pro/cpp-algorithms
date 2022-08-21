#pragma once

inline int power(int a, int b) {
	if ((a == 0 && b == 0) || b == 0) { return 1; }
	if (b == 1) { return a; }

	int v = b % 2 == 0 ? 1 : a;
	// if power is even i^n = (i*i)^(n/2)
	// if power is odd i^n = i*(i)^(n/2)
	return v * power(a * a, b / 2);
}

template<typename T, typename N, typename O>
T power(T x, N n, O op) {
	if (n == 0) { return identity_element(op); }

	while ((n & 1) == 0) {
		n >>= 1;
		x = op(x, x);
	}

	T result = x;
	n >>= 1;

	while (n != 0) {
		x = op(x, x);

		if ((n & 1) != 0) { result = op(result, x); }

		n >>= 1;
	}

	return result;
}
