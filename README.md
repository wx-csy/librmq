# librmq

This toy library is a fast implementation that solves the static Range Minimum Query (RMQ) problem.

## Range Minimum Query (RMQ) Problem

Given an array `a[0...n-1]` and multiple queries of the form `(l, r)`. For each query, find (the index of) the minimum element in subarray `a[l...r-1]`.

Naive solution works in O(n) per query with constant preprocessing time. Also there is a classic data structure called sparse table which works in O(1) per query with O(n log n) preprocessing time. In theoretical computer science, there is an algorithm where each query can be answered in O(1) time after O(n) preprocessing; the algorithm involves a technique called indirection, a.k.a. the method of four Russians. However, direct implementation of the theoretical algorithm hardly outperforms the O(n log n)/O(1) sparse table solution. So we need to adapt the original algorithm and fine tune the parameters. A short explanation of the theoretical part of the solution can be found in http://aequa.me/index.php/2019/07/25/range-minimum-query-and-lowest-common-ancestor-in-on-o1-time-an-overview/.

## RMQ Solvers

Currently, this library implements three RMQ solvers: naive, sparse-table and indirection. They have the uniform interface:

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

## Evaluation

### Environment

- Hardware: Lenovo Thinkpad T480 laptop, Intel(R) Core(TM) i5-8250U CPU 
@ 1.60GHz, 8G RAM
- Operation System: Ubuntu 18.04.2 LTS
- RMQ Solver: librmq 1.0
- C++ Compiler: g++ (Ubuntu 7.4.0-ubuntu1\~18.04.1) 7.4.0
- Compiler Flags: `g++ -O3 -std=c++14 -ggdb $(SRCS) -o $(DEST)`

### Dataset

Each test case consists of an array of length 10000000 and 10000000 queries. The array of the data and the queries are all generated uniformly at random.

### Results

- tprep: the preprocessing time (in second)
- tquery: the total query time (in second)
- MQPS: million queries per second
- ctp/ctq: the amortized preprocessing cost per element over the average cost per query

#### Sparse Table

Round | tprep | tquery | MQPS | ctp/ctq
:-: | :-: | :-: | :-: | :-:
0 | 1.870001 |	1.064212 |	9.396624 |	1.757170 
1 | 1.862305 |	1.119636 |	8.931474 |	1.663313 
2 | 1.909492 |	1.054692 |	9.481441 |	1.810474 
3 | 1.854921 |	1.112837 |	8.986042 |	1.666840 
4 | 1.856866 |	1.053522 |	9.491971 |	1.762532 
5 | 1.855417 |	1.117154 |	8.951317 |	1.660843 
6 | 1.875308 |	1.052386 |	9.502217 |	1.781958 
7 | 1.855586 |	1.114908 |	8.969350 |	1.664340 
8 | 1.855936 |	1.051602 |	9.509301 |	1.764865 
9 | 1.859268 |	1.122524 |	8.908495 |	1.656328 
avg | 1.865510 | 1.086347 | 9.205159 | 1.717232

#### Indirection

Round | tprep | tquery | MQPS | ctp/ctq
:-: | :-: | :-: | :-: | :-:
0 | 0.429709 |	3.028783 |	3.301656 | 	0.141875 
1 | 0.430829 |	2.887973 |	3.462636 | 	0.149180 
2 | 0.429891 |	2.841680 |	3.519045 | 	0.151281 
3 | 0.440895 |	2.877670 |	3.475034 | 	0.153212 
4 | 0.430051 |	2.839728 |	3.521464 | 	0.151441 
5 | 0.429124 |	2.812179 |	3.555961 | 	0.152595 
6 | 0.429231 |	2.845471 |	3.514357 | 	0.150847 
7 | 0.429544 |	2.846401 |	3.513208 | 	0.150908 
8 | 0.429804 |	2.876509 |	3.476436 | 	0.149419 
9 | 0.430153 |	2.893870 |	3.455580 | 	0.148643 
avg | 0.430923 |	2.875026 |	3.478229 |	0.149885
