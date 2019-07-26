#include <algorithm>
#include "hardcode.h"
#include "librmq.h"

namespace librmq {

static inline int select_segment(const int data[]) {
    int ptr = 0;
    int crit;
    while ((crit = decision_tree_nodes[ptr].crit) > 0) { 
        ptr = decision_tree_nodes[ptr].son[data[crit / SEGLEN] < data[crit % SEGLEN]];
    }
    return decision_tree_nodes[ptr].tree;
}

rmq_ind::rmq_ind(const size_t n, const int * const data) :
        n(n), data(data) {
    nr_blocks = (n + SEGLEN - 1) / SEGLEN;
    size_t *segdata = new size_t[nr_blocks]; // ownership transferred to ST
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
    if (lb != rb) { // speculative policy
        size_t ret = st.query(lb, std::min(nr_blocks, rb + 1));
        if (ret >= l and ret < r) return ret;
    } else {
        size_t base = lb * SEGLEN;
        return lookup_table[segid[lb]][l - base][r - base - 1] + base;
    } 
    
    size_t lbase = lb * SEGLEN, rbase = rb * SEGLEN;
    size_t v = lookup_suf[segid[lb]][l - lbase] + lbase;
    if (r != rbase) {
        size_t v2 = lookup_pref[segid[rb]][r - rbase - 1] + rbase;
        if (data[v2] < data[v]) v = v2;
    }
    if (lb + 1 == rb) return v;
    size_t vv = st.query(lb + 1, rb);
    return data[vv] < data[v] ? vv : v;
}

rmq_ind::~rmq_ind() {
    delete[] segid;
}

}

