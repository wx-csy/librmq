# librmq

This toy library is a fast implementation that solves the static Range Minimum Query (RMQ) problem.

It is still under development.

## Range Minimum Query (RMQ) Problem

Given an array `a[0...n-1]` and multiple queries of the form `(l, r)`. For each query, find (the index of) the minimum element in subarray `a[l...r-1]`.

Theoretically, each query can be answered in O(1) time after O(n) preprocessing, which is optimal. However, direct implementation of the theoretical algorithm hardly outperforms the classic O(n log n)/O(1) sparse table solution. So we need to adapt the original algorithm and fine tune the parameters.

A short explanation of the theoretical part of the solution can be found in http://aequa.me/index.php/2019/07/25/range-minimum-query-and-lowest-common-ancestor-in-on-o1-time-an-overview/.
