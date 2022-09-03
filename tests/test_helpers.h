#pragma once
#include <gtest/gtest.h>
#include <stdint.h>
#include <vector>

struct Cl {
	const char* file;
	std::size_t line;
};

#define CL             \
	Cl {                 \
		__FILE__, __LINE__ \
	}

template<typename T>
concept Formatable = requires(T x) {
	std::formatter<T>::format;// required class template specialization
};

template<Formatable T>// type must support format
void test_arrays(const std::vector<T>& expected, const std::vector<T>& actual, Cl cl = CL) {
	const std::size_t MAX_PRINT_LINE = 3;
	std::size_t size_of_expected = std::size(expected);
	std::size_t size_of_actual = std::size(actual);
	auto [minSize, maxSize] = std::minmax(size_of_expected, size_of_actual);

	bool wasFailed = false;
	std::size_t currentPrintCount = 0;
	std::string message;
	if (size_of_expected != size_of_actual) {
		wasFailed = true;
		message = std::format("Expect size {} but actual size is {}:", size_of_expected, size_of_actual);

		// print details
		const std::vector<T>& vMax = size_of_expected > size_of_actual ? expected : actual;
		const std::vector<T>& vMin = size_of_expected > size_of_actual ? actual : expected;
		std::string info = size_of_expected > size_of_actual ? "expect" : "also get";
		for (std::size_t i = 0; i < minSize; ++i) {
			if (expected[i] != actual[i]) {
				if (currentPrintCount < MAX_PRINT_LINE) {
					message += std::format("\n   expect [{}] but actual is [{}] at index {};", expected[i], actual[i], i);
				}
				currentPrintCount++;
			}
		}
		for (std::size_t i = minSize; i < maxSize; ++i) {
			if (currentPrintCount < MAX_PRINT_LINE) { message += std::format("\n   {} [{}] at index {};", info, vMax[i], i); }
			currentPrintCount++;
		}
	} else {// same size
		message = "Arrays mismatch: ";
		for (std::size_t i = 0; i < size_of_expected; ++i) {
			if (expected[i] != actual[i]) {
				wasFailed = true;
				if (currentPrintCount < MAX_PRINT_LINE) {
					message += std::format("\n   expect [{}] but actual is [{}] at index {};", expected[i], actual[i], i);
				}
				currentPrintCount++;
			}
		}
	}
	if (currentPrintCount > MAX_PRINT_LINE) {
		message += std::format("\n   ... and {} more", currentPrintCount - MAX_PRINT_LINE);
	}
	if (wasFailed) { GTEST_FAIL_AT(cl.file, cl.line) << message; }
}

#define EXPECT_ARR_EQ(expected, actual) test_arrays(expected, actual, CL)

#define SINGLE_ARG(...) __VA_ARGS__
