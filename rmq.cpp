#include <algorithm>
#include "hardcode.h"
#include "rmq.h"
#include <cassert>

static inline int select_segment(const int data[]) {
    int ptr = 0;
    while (nodes[ptr].crit > 0) { 
        int crit = nodes[ptr].crit;
        ptr = nodes[ptr].son[data[crit / SEGLEN] < data[crit % SEGLEN]];
    }
    return nodes[ptr].tree;
}

rmq::rmq(const size_t n, const int * const data) :
        n(n), data(data) {
    nr_blocks = (n + SEGLEN - 1) / SEGLEN;
    int *segdata = new int[nr_blocks];
    segid = new int[nr_blocks];
    size_t ptr = 0;
    for (int i = 0; ptr + SEGLEN <= n; ptr += SEGLEN, i++) {
        segdata[i] = std::min_element(data + ptr, data + ptr + SEGLEN) - data;
        segid[i] = select_segment(data + ptr);
    }
    if (ptr != n) {
        int rdata[SEGLEN] = {0};
        for (int i = 0; ptr + i < n; i++) rdata[i] = data[ptr + i];
        segdata[nr_blocks - 1] = std::min_element(data + ptr, data + n) - data;
        segid[nr_blocks - 1] = select_segment(rdata);
    }
    st.init(nr_blocks, data, segdata);
}

int rmq::query(int l, int r) {
    int lb = l / SEGLEN, rb = r / SEGLEN;
    if (l >= r) return -1;
    if (lb == rb) {
        int base = lb * SEGLEN;
        return lookup[segid[lb]].result[l - base][r - base - 1] + base;
    } else {
        int lbase = lb * SEGLEN, rbase = rb * SEGLEN;
        int v = lookup[segid[lb]].result[l - lbase][SEGLEN - 1] + lbase;
        if (r != rbase) {
            int v2 = lookup[segid[rb]].result[0][r - rbase - 1] + rbase;
            if (data[v2] < data[v]) v = v2;
        }
        if (lb + 1 == rb) return v;
        int vv = st.query(lb + 1, rb);
        return data[vv] < data[v] ? vv : v;
    }
}

rmq::~rmq() {
    delete[] segid;
}

