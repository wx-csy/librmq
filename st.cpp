#include <numeric>
#include "librmq.h"

namespace librmq {

void rmq_st::init(const size_t n, const int * const data, 
        size_t * const first) {
    this->n = n; this->data = data;
    if (first) {
        st[0] = first; 
    } else { 
        st[0] = new size_t[n]; std::iota(st[0], st[0] + n, size_t(0));
    }
    for (size_t lv = 0; (2 << lv) <= n; lv++) {
        st[lv+1] = new size_t[n - (2 << lv) + 1];
        for (size_t i = 0; i + (2 << lv) <= n; i++) {
            size_t p1 = st[lv][i], p2 = st[lv][i + (1 << lv)];
            st[lv+1][i] = (data[p1] < data[p2] ? p1 : p2);
        }
    }
}

rmq_st::~rmq_st() {
    for (size_t *st_entry : st) delete[] st_entry;
}

static inline size_t fast_log2(size_t length) {
    return sizeof(size_t) * 8 - 1 - __builtin_clzll(length);
}

size_t rmq_st::query(size_t l, size_t r) {
    size_t level = fast_log2(r - l);
    size_t p1 = st[level][l], p2 = st[level][r - (1 << level)];
    return data[p1] < data[p2] ? p1 : p2; 
}

}
