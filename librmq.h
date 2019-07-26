#ifndef __RMQ_H__
#define __RMQ_H__

#include <cstdint>

namespace librmq {

using std::size_t;

class rmq_naive {
    size_t n;
    const int *data = nullptr;    

public:
    rmq_naive(size_t n, const int *data);
    ~rmq_naive() = default;
    size_t query(size_t l, size_t r);
};

class rmq_st {
    static constexpr int MAXLOGN = 60;
    size_t n;
    size_t *st[MAXLOGN] = {};
    const int *data = nullptr;
   
public:
    rmq_st() { n = 0; }
    rmq_st(size_t n, const int *data) { init(n, data); }
    void init(size_t n, const int *data, size_t *first = nullptr);
    ~rmq_st();
    size_t query(size_t l, size_t r);
};

class rmq_ind {
    size_t n, nr_blocks;
    const int *data;
    int *segid;
    rmq_st st;

public:
    rmq_ind(size_t n, const int *data);
    ~rmq_ind();
    size_t query(size_t l, size_t r);
};

}
#endif
