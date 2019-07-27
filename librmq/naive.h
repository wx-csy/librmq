#ifndef __LIBRMQ_NAIVE_H__
#define __LIBRMQ_NAIVE_H__

#include <cstdint>
#include <algorithm>

namespace librmq {

using std::size_t;

class rmq_naive {
    size_t n;
    const int *data = nullptr;    

public:
    rmq_naive(size_t n, const int *data) : 
        n(n), data(data) {}

    size_t query(size_t l, size_t r) {
        return std::min_element(data + l, data + r) - data;
    }
};

}

#endif