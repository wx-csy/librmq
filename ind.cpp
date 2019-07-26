#include <algorithm>
#include "hardcode.h"
#include "librmq.h"

namespace librmq {

static inline int select_segment(const int data[]) {
    int ptr = 0;
    while (nodes[ptr].crit > 0) { 
        int crit = nodes[ptr].crit;
        ptr = nodes[ptr].son[data[crit / SEGLEN] < data[crit % SEGLEN]];
    }
    return nodes[ptr].tree;
}

rmq_ind::rmq_ind(const size_t n, const int * const data) :
        n(n), data(data) {
    nr_blocks = (n + SEGLEN - 1) / SEGLEN;
    size_t *segdata = new size_t[nr_blocks];
    segid = new int[nr_blocks];
    size_t ptr = 0;
    for (size_t i = 0; ptr + SEGLEN <= n; ptr += SEGLEN, i++) {
        segdata[i] = std::min_element(data + ptr, data + ptr + SEGLEN) - data;
        segid[i] = select_segment(data + ptr);
    }
    if (ptr != n) {
        int rdata[SEGLEN] = {0};
        for (size_t i = 0; ptr + i < n; i++) rdata[i] = data[ptr + i];
        segdata[nr_blocks - 1] = std::min_element(data + ptr, data + n) - data;
        segid[nr_blocks - 1] = select_segment(rdata);
    }
    st.init(nr_blocks, data, segdata);
}

size_t rmq_ind::query(size_t l, size_t r) {
    size_t lb = l / SEGLEN, rb = r / SEGLEN;
    if (l >= r) return -1;
    if (lb == rb) {
        size_t base = lb * SEGLEN;
        return lookup[segid[lb]].result[l - base][r - base - 1] + base;
    } else {
        size_t lbase = lb * SEGLEN, rbase = rb * SEGLEN;
        size_t v = lookup[segid[lb]].result[l - lbase][SEGLEN - 1] + lbase;
        if (r != rbase) {
            size_t v2 = lookup[segid[rb]].result[0][r - rbase - 1] + rbase;
            if (data[v2] < data[v]) v = v2;
        }
        if (lb + 1 == rb) return v;
        size_t vv = st.query(lb + 1, rb);
        return data[vv] < data[v] ? vv : v;
    }
}

rmq_ind::~rmq_ind() {
    delete[] segid;
}

}

