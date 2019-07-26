#include "librmq.h"
#include <algorithm>

namespace librmq {

rmq_naive::rmq_naive(size_t n, const int *data) :
        n(n), data(data) {}

size_t rmq_naive::query(size_t l, size_t r) {
    return std::min_element(data + l, data + r) - data;
}

}
