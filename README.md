# C++ algorithms

C++ algorithms implementations

### Types
1. **Linked list** ([*types/list.h*](/src/types/list.h)) - custom ram allocator implemented. (see **Text editor** example )
1. **Big integer type** ([*types/big_int.h*](/src/types/big_int.h))  - type that can hold number more than 64bit integers. (only unsigned numbers, only sum operator implemented as example)
1. **Graph** ([*types/big_int.h*](/src5/types/graph.h))  - vertices, edges, and search in graphs.
### Iterators
1. **Zip** ([*iterators/zip.h*](/src/iterators/zip.h)) - zip iterator: combine two arrays into array of pairs.
1. **Enumerate** ([*iterators/enumerate.h*](/src/iterators/enumerate.h)) - allow to iterate over indexes of array in for-range loop (like enumerate in python).
### Algorithms
1. **Insertion sort** ([*algorithms/insertion_sort.h*](/src/algorithms/insertion_sort.h)) - simple insertion sorting algorithm
1. **Fibonacci function** ([*algorithms/fibonacci.h*](/src/algorithms/fibonacci.h)) - fibonacci algorithm without memoization, but no unnecessary calls within one calculation.
1. **Binary search** ([*algorithms/binary_search.h*](/src/algorithms/binary_search.h)) - binary search implementation.
1. **Greatest common divisor** ([*algorithms/gcd.h*](/src/algorithms/gcd.h)) - greatest common divisor implementation.
1. **Power of number**  ([*algorithms/power.h*](/src/algorithms/power.h)) - calculate power of number.
1. **Palindrom test**  ([*algorithms/palindrom.h*](/src/algorithms/palindrom.h)) - test string on palindrom.
1. **Quick sort**  ([*algorithms/quick_sort.h*](/src/algorithms/quick_sort.h)) - quick sort implementation.
1. **Combinatorics permutations**  ([*algorithms/combinatorics.h*](/src/algorithms/combinatorics.h)) - find all permutations of string.
1. **AVL Tree**  ([*algorithms/tree.h*](/src/algorithms/tree.h)) - AVL tree based on binary tree. Note: not all algorithms is implemented.
1. **Gradient descent**  ([*algorithms/gradient_descent.h*](/src/algorithms/gradient_descent.h)) - Gradient descent search optimization algorithm
### Examples
1. **Text editor example** ([*examples/editor.h*](/src/examples/editor.h)) - example of console text editor  based on **Linked list** implementation.
1. **Hanoi tower example** ([*examples/hanoi.h*](/src/examples/hanoi.h)) - example of solving hanoi tower puzzle by recursion.
1. **Value to binary example** ([*examples/value_to_binary.h*](/src/examples/value_to_binary.h)) - recursive function that convert integer value to binary as string.
1. **Coin exchange algorithm** ([*examples/coins.h*](/src/examples/coins.h)) - backtrack search algorithm implementation of coins exchange. Used static time polymorphism with variant type.
1. **Maze solver** ([*examples/maze.h*](/src/examples/maze.h)) - backtrack search the exit from maze.  
1. **Maze generator** ([*examples/maze_generator.h*](/src/examples/maze_generator.h)) - euler maze generator.  
### Patterns
1. **Observer pattern** ([*patterns/observer.h*](/src/patterns/observer.h)) - observer without dynamic memory allocation, using chain of responsibility (linked list of handlers).
### Multithread
1. **Future utils** ([*multithread/future_utils.h*](/src/multithread/future_utils.h)) - some functions to work with futures.
1. **Job utils** ([*multithread/future_utils.h*](/src/multithread/future_utils.h)) - some functions to work with futures.
