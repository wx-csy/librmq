# librmq

This toy library implements several algorithms and data structures that solves the static Range Minimum Query (RMQ) problem.

## Range Minimum Query (RMQ) Problem

Given an array `a[0...n-1]` and multiple queries of the form `(l, r)`. For each query, find (the index of) the minimum element in subarray `a[l...r-1]`.

Naive solution works in O(n) per query with constant preprocessing time. Also there is a classic data structure called sparse table which works in O(1) per query with O(n log n) preprocessing time. In theoretical computer science, there is an algorithm where each query can be answered in O(1) time after O(n) preprocessing; the algorithm involves a technique called indirection, a.k.a. the method of four Russians. However, direct implementation of the theoretical algorithm hardly outperforms the O(n log n)/O(1) sparse table solution. So we need to adapt the original algorithm and fine tune the parameters. A short explanation of the theoretical part of the solution can be found in http://aequa.me/index.php/2019/07/25/range-minimum-query-and-lowest-common-ancestor-in-on-o1-time-an-overview/.

## RMQ Solvers

Currently, this library implements three RMQ solvers: naive, sparse-table, indirection and simple block decomposition. They have the uniform interface:

``` c++
class solver {
    solver(size_t n, const int *data);
    size_t query(size_t l, size_t r);
};
```

- `solver(size_t n, const int *data)`: construct a solver instance with array `data` and length `n`;
- `size_t query(size_t l, size_t r)`: query the minimum element in range l ... r - 1, return the index of the minimum element; when there are multiple minimum elements, which index is returned is unspecified. It is undefined when l >= r, or part of the range queried is out of the range of the array.

### Naive Solver: rmq_naive

The naive solver solves the problem in O(1)/O(n) time.

### Sparse Table: rmq_st

The sparse table solver solves the problem in O(n log n)/O(1) time.

### Indirection: rmq_ind

The indirection solver solves the problem in O(n)/O(1) time. It uses `rmq_st` as the underlying RMQ data structure.

### Simple Block Decomposition: rmq_block

This algorithm is similar to indirection, but it does not preprocess a lookup table. After O(n) preprocessing time, it answers each query in O(1) time in average.

## Evaluation

This page maintains the evaluation results for the current developing version. Evaluations for old version can be found [here](doc/eval.md).

### Environment

- Hardware: Lenovo ThinkPad T480 laptop, Intel(R) Core(TM) i5-8250U CPU 
@ 1.60GHz, 8G RAM
- Operation System: Ubuntu 18.04.2 LTS
- C++ Compiler: g++ (Ubuntu 7.4.0-ubuntu1\~18.04.1) 7.4.0
- Compiler Flags: `g++ -O3 -std=c++14 -ggdb $(SRCS) -o $(DEST)`

### Dataset

Each test case consists of an array of length 10000000 and 10000000 queries. The array of the data and the queries are all generated uniformly at random.

### Results

- tprep: the preprocessing time (in second)
- tquery: the total query time (in second)
- MQPS: million queries per second
- ctp/ctq: the amortized preprocessing cost per element over the average cost per query

Solver | tprep | tquery | MQPS | ctp/ctq
:-: | :-: | :-: | :-: | :-:
`rmq_st` | 0.840555 | 0.649688 | 15.392004 | 1.293783
`rmq_ind` | 0.259122 | 0.540844 | 18.489620 | 0.479107
`rmq_block` | 0.132950 | 0.095457 | 102.018955 | 1.356342

## Changelog

- ADD: implement a speculative policy in query (~200% speedup)
- ADD: implement a simple block decomposition algorithm `rmq_block` (thanks to Songyang Chen)

The changelog for old versions can be found [here](doc/changelog.md)
