#ifndef __LIBRMQ_NAIVE_H__
#define __LIBRMQ_NAIVE_H__

#include <cstdint>
#include <algorithm>

namespace librmq {

using std::size_t;

template <typename T>
class rmq_naive {
    size_t n;
    const T *data = nullptr;    

public:
    rmq_naive(size_t n, const T *data) : 
        n(n), data(data) {}

    size_t query(size_t l, size_t r) {
        return std::min_element(data + l, data + r) - data;
    }
};

}

#endif