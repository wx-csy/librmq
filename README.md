# librmq

This toy library implements several algorithms and data structures that solves the static Range Minimum Query (RMQ) problem.

## Range Minimum Query (RMQ) Problem

Given an array `a[0 ... n-1]` and multiple queries of the form `(l, r)`. For each query, find (the index of) the minimum element in subarray `a[l ... r-1]`.

Naive solution works in O(n) per query with constant preprocessing time. Also there is a classic data structure called sparse table which works in O(1) per query with O(n log n) preprocessing time. In theoretical computer science, there is an algorithm where each query can be answered in O(1) time after O(n) preprocessing; the algorithm involves a technique called indirection, a.k.a. the method of four Russians. However, direct implementation of the theoretical algorithm hardly outperforms the O(n log n)/O(1) sparse table solution. So we need to adapt the original algorithm and fine tune the parameters. A short explanation of the theoretical part of the solution can be found in http://aequa.me/index.php/2019/07/25/range-minimum-query-and-lowest-common-ancestor-in-on-o1-time-an-overview/.

## RMQ Solvers

Currently, this library implements five RMQ solvers: naive, ZKW segment tree, sparse-table, indirection and simple block decomposition. They have the uniform interface:

``` c++
class solver {
    solver(size_t n, const int *data);
    size_t query(size_t l, size_t r);
};
```

- `solver(size_t n, const int *data)`: construct a solver instance with array `data` and length `n`;
- `size_t query(size_t l, size_t r)`: query the minimum element in range l ... r - 1, return the index of the minimum element; when there are multiple minimum elements, which index is returned is unspecified. It is undefined when l >= r, or part of the range queried is out of the range of the array.

### Naive Solver: rmq_naive

The naive solver solves the problem in O(1)/O(n) time. In preprocessing stage, it simply stores a pointer to the data. For each query, it performs a linear scan and find the minimum element.

### ZKW Segment Tree: rmq_zkw

This data structure solves the range minimum in O(n)/O(log n) time.

### Sparse Table: rmq_st

The sparse table solver solves the problem in O(n log n)/O(1) time. It implements the sparse table, where answers for intervals of length power of 2 are precomputed. There are O(log n) different lengths, and for each length, there are O(n) intervals, so the preprocessing time and memory cost is O(n log n). For each query, it uses two intervals of length power of 2 to cover the entire interval. Specifically, for query `a[l ... r-1]`, let k=floor(log2(r-l)), then the two intervals used are `a[l ... l+2^k]` and `a[r-1-2^k ... r-1]`Due to the idempotence of minimum operation, the overlapped part will not affect the answer. So the complexity is O(1) per query.

### Indirection: rmq_ind

The indirection solver solves the problem in O(n)/O(1) time. In the current implementation, it partitions the entire array into segments of length 8. For each segment, extract its minimum element and build a sparse table on them. Also, for each segment, preprocess all possible queries entirely wthin this segment. There are actually not too much essentially different segments (only 1430 --- the 8th Catalan number). Actually all within segment queries are hardcoded. Hence, for each query, if it is entirely within a segment, it can be answered directly by lookup table; otherwise, the interval can be partitioned into two within-segment queries and a query over consecutive segments, and the latter one can be done by looking up the sparse table. The overall time is still constant.

In the current implementation, a speculative heuristic is added: first query the minimum consecutive segments that contain the entire query interval; if the minimum element is in the query interval, then return the answer directly. This speeds up the query by ~200% in average.

### Simple Block Decomposition: rmq_block

This algorithm is similar to indirection, but it does not precompute a lookup table. After O(n) preprocessing time, it answers each query in O(1) time in average.

It also partitions the array into segments and build a sparse table over the segments, but the segment is much larger than than that in `rmq_ind`. The difference is, it does not preprocess a lookup table; it just preprocesses prefix minimum value and suffix minimum value for each block. For queries spanning multiple blocks, the partition in `rmq_id` still works; for queries that is entirely in one segment, just run a linear scan; such case is not very often in average. Also the speculative heuristic is still used.

Thanks Songyang Chen for providing this wonderful idea!

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
`rmq_zkw` | 0.157691 | 4.129427 | 2.421643 | 0.038187
`rmq_st` | 0.840610 | 0.665245 | 15.032049 | 1.263609
`rmq_ind` | 0.260333 | 0.397816 | 25.137275 | 0.654406
`rmq_block` | 0.136653 | 0.066277  | 150.882117 | 2.061843

## Changelog

The changelog for old versions can be found [here](doc/changelog.md).
