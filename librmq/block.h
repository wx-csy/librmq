#ifndef __LIBRMQ_BLOCK_H__
#define __LIBRMQ_BLOCK_H__

#include <algorithm>
#include "hardcode.h"
#include "st.h"

namespace librmq {

template <typename T>
class rmq_block {
    static constexpr size_t BLOCKSZ = 1024;
    size_t n, nr_blocks;
    const T *data;
    size_t *pref, *suf;
    rmq_st<T> st;
    
    void init_block(const T *data, size_t si) {
        size_t ptr = si * BLOCKSZ;
        for (int i = 0; i < BLOCKSZ; i++) {
            if (i) {
                pref[ptr + i] = data[i] < data[pref[ptr + i - 1]] ? 
                    i : pref[ptr + i - 1];
            } else {
                pref[ptr + i] = i;
            }
        }
        for (int i = BLOCKSZ - 1; i >= 0; i--) {
            if (i == BLOCKSZ - 1) {
                suf[ptr + i] = i;
            } else {
                suf[ptr + i] = data[i] < data[suf[ptr + i + 1]] ?
                    i : suf[ptr + i + 1];
            }
        }
        for (int i = 0; i < BLOCKSZ; i++) pref[ptr + i] += ptr;
        for (int i = 0; i < BLOCKSZ; i++) suf[ptr + i] += ptr;
    }

public:
    rmq_block(size_t n, const T *data) :
            n(n), data(data) {
        nr_blocks = (n + BLOCKSZ - 1) / BLOCKSZ;
        size_t *segdata = new size_t[nr_blocks];
        pref = new size_t[nr_blocks * BLOCKSZ]; 
        suf = new size_t[nr_blocks * BLOCKSZ];
        size_t ptr = 0;
        for (size_t si = 0; ptr + BLOCKSZ <= n; ptr += BLOCKSZ, si++) {
            segdata[si] = std::min_element(data + ptr, data + ptr + BLOCKSZ) - data;
            init_block(data + ptr, si);
        }
        if (ptr != n) {
            T rdata[BLOCKSZ] = {0};
            for (size_t i = 0; ptr + i < n; i++) rdata[i] = data[ptr + i];
            segdata[nr_blocks - 1] = std::min_element(data + ptr, data + n) - data;
            init_block(rdata, nr_blocks - 1);
        }
        st.init(nr_blocks, data, segdata);
    }

    ~rmq_block() {
        delete[] pref; delete[] suf;
    }

    size_t query(size_t l, size_t r) {
        size_t lb = l / BLOCKSZ, rb = r / BLOCKSZ;
        if (lb != rb) { // speculative policy
            size_t ret = st.query(lb, std::min(nr_blocks, rb + 1));
            if (ret >= l and ret < r) return ret;
        } else {
            return std::min_element(data + l, data + r) - data;
        }
        size_t lbase = lb * BLOCKSZ, rbase = rb * BLOCKSZ;
        size_t v = suf[l];
        if (r != rbase) {
            size_t v2 = pref[r - 1];
            if (data[v2] < data[v]) v = v2;
        }
        if (lb + 1 == rb) return v;
        size_t vv = st.query(lb + 1, rb);
        return data[vv] < data[v] ? vv : v;
    }
};

}


#endif
