# Evaluation Log

## Version 1.0

### Environment

- Hardware: Lenovo ThinkPad T480 laptop, Intel(R) Core(TM) i5-8250U CPU 
@ 1.60GHz, 8G RAM
- Operation System: Ubuntu 18.04.2 LTS
- C++ Compiler: g++ (Ubuntu 7.4.0-ubuntu1\~18.04.1) 7.4.0
- Compiler Flags: `g++ -O3 -std=c++14 -ggdb $(SRCS) -o $(DEST)`

### Dataset

Each test case consists of an array of length 10000000 and 10000000 queries. The array of the data and the queries are all generated uniformly at random. The final result is obtained by averaging the result of 10 runs.

### Results

- tprep: the preprocessing time (in second)
- tquery: the total query time (in second)
- MQPS: million queries per second
- ctp/ctq: the amortized preprocessing cost per element over the average cost per query

Solver | tprep | tquery | MQPS | ctp/ctq
:-: | :-: | :-: | :-: | :-:
`rmq_st` | 0.843508 | 0.637704 | 15.681257 | 1.322727 
`rmq_ind` | 0.259908 | 1.586838 | 6.301841 | 0.163223
