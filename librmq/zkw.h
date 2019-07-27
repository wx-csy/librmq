#ifndef __LIBRMQ_ZKW_H__
#define __LIBRMQ_ZKW_H__

#include <numeric>
#include <algorithm>
#include <cstddef>

namespace librmq {

using std::size_t;

class rmq_zkw {
    size_t n, ntot;
    const int *data;
    size_t *tr;

public:
    rmq_zkw(size_t n, const int *data) : n(n), data(data) {
        ntot = n;
        while (ntot & (ntot - 1)) ntot += ntot & -ntot;
        tr = new size_t[ntot << 1];
        std::iota(tr + ntot, tr + ntot + n, 0);
        std::fill(tr + ntot + n, tr + ntot + ntot, 0);
        for (size_t i = ntot - 1; i > 0; i--)
            tr[i] = data[tr[i<<1]] < data[tr[i<<1|1]] ? tr[i<<1] : tr[i<<1|1];
    }

    ~rmq_zkw() {
        delete[] tr;
    }

    size_t query(size_t l, size_t r) {
        size_t ret = l;
        l += ntot - 1;
        r += ntot;
        while (l ^ r ^ 1) {
            if ((~l & 1) && data[tr[l ^ 1]] < data[ret]) ret = tr[l ^ 1]; 
            if (( r & 1) && data[tr[r ^ 1]] < data[ret]) ret = tr[r ^ 1];
            l >>= 1;
            r >>= 1;
        }
        return ret;
    }
};

}

#endif