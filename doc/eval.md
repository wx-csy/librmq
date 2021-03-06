# Evaluation Log

## Version 2.0

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
`rmq_zkw` | 0.157009 | 4.013887 | 2.491351 | 0.039117
`rmq_st` | 0.840631 | 0.633228 | 15.792090 | 1.327533
`rmq_ind` | 0.260702 | 0.389355 | 25.683508 | 0.669574
`rmq_block` | 0.135279 | 0.079615  | 125.604289 | 1.699163

## Version 1.1

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
