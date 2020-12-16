# C++ algorithms

[![codecov](https://codecov.io/gh/Loggi-pro/cpp-algorithms/branch/master/graph/badge.svg?token=sbY7YfIaUg)](https://codecov.io/gh/Loggi-pro/cpp-algorithms) 
[![Build Status](https://travis-ci.com/loggi-pro/cpp-algorithms.svg?branch=master)](https://travis-ci.com/loggi-pro/cpp-algorithms) 
[![Build status](https://ci.appveyor.com/api/projects/status/jprik3uyr8162qkg/branch/master?svg=true)](https://ci.appveyor.com/project/Loggi-pro/cpp-algorithms/branch/master)


C++ algorithms implementations

###Types
1. **Linked list** ([*types/list.h*](/src/types/list.h)) - custom ram allocator implemented. (see **Text editor** example )
2. **Big integer type** ([*types/big_int.h*](/src/types/big_int.h))  - type that can hold number more than 64bit integers. (only unsigned numbers, only sum operator implemented as example)
### Algorithms
1. **Insertion sort** ([*algorithms/insertion_sort.h*](/src/algorithms/insertion_sort.h)) - simple insertion sorting algorithm
1. **Fibonacci function** ([*algorithms/fibonacci.h*](/src/algorithms/fibonacci.h)) - fibonacci algorithm without memoization, but no unnecessary calls within one calculation.
### Examples
1. **Text editor example** ([*examples/editor.h*](/src/examples/editor.h)) - example of console text editor  based on **Linked list** implementation.

