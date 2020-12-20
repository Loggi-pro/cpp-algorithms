#pragma once

inline int power(int a, int b) {
	if ((a == 0 && b == 0) || b == 0) { return 1; }
	if (b == 1) { return a; }

	if (b % 2 == 0) {
		return 1 * power(a * a, b / 2);// if power is even i^n = (i*i)^(n/2)
	} else {
		return a * power(a * a, b / 2);// if power is odd i^n = i*(i)^(n/2)
	}
}
