# C++ algorithms

[![codecov](https://codecov.io/gh/Loggi-pro/cpp-algorithms/branch/master/graph/badge.svg?token=sbY7YfIaUg)](https://codecov.io/gh/Loggi-pro/cpp-algorithms) 
[![Build Status](https://travis-ci.com/loggi-pro/cpp-algorithms.svg?branch=master)](https://travis-ci.com/loggi-pro/cpp-algorithms) 
[![Build status](https://ci.appveyor.com/api/projects/status/jprik3uyr8162qkg/branch/master?svg=true)](https://ci.appveyor.com/project/Loggi-pro/cpp-algorithms/branch/master)


C++ algorithms implementations

### Types
1. **Linked list** ([*types/list.h*](/src/types/list.h)) - custom ram allocator implemented. (see **Text editor** example )
2. **Big integer type** ([*types/big_int.h*](/src/types/big_int.h))  - type that can hold number more than 64bit integers. (only unsigned numbers, only sum operator implemented as example)
### Algorithms
1. **Insertion sort** ([*algorithms/insertion_sort.h*](/src/algorithms/insertion_sort.h)) - simple insertion sorting algorithm
2. **Fibonacci function** ([*algorithms/fibonacci.h*](/src/algorithms/fibonacci.h)) - fibonacci algorithm without memoization, but no unnecessary calls within one calculation.
3. **Binary search** ([*algorithms/binary_search.h*](/src/algorithms/binary_search.h)) - binary search implementation.
4. **Greatest common divisor** ([*algorithms/gcd.h*](/src/algorithms/gcd.h)) - greatest common divisor implementation.
5. **Power of number**  ([*algorithms/power.h*](/src/algorithms/power.h)) - calculate power of number.
6. **Palindrom test**  ([*algorithms/palindrom.h*](/src/algorithms/palindrom.h)) - test string on palindrom.
7. **Quick sort**  ([*algorithms/quick_sort.h*](/src/algorithms/quick_sort.h)) - quick sort implementation.
7. **Combinatorics permutations**  ([*algorithms/combinatorics.h*](/src/algorithms/combinatorics.h)) - find all permutations of string.
### Examples
1. **Text editor example** ([*examples/editor.h*](/src/examples/editor.h)) - example of console text editor  based on **Linked list** implementation.
2. **Hanoi tower example** ([*examples/hanoi.h*](/src/examples/hanoi.h)) - example of solving hanoi tower puzzle by recursion.
3. **Value to binary example** ([*examples/value_to_binary.h*](/src/examples/value_to_binary.h)) - recursive function that convert integer value to binary as string.
3. **Coin exchange algorithm** ([*examples/coins.h*](/src/examples/coins.h)) - backtrack search algorithm implementation of coins exchange. Used static time polymorphism with variant type.
3. **Maze solver** ([*examples/maze.h*](/src/examples/maze.h)) - backtrack search the exit from maze.  
