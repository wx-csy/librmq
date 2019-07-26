#ifndef __RMQ_H__
#define __RMQ_H__

#include "st.h"

struct rmq {
    size_t n, nr_blocks;
    const int *data;
    int *segid;
    sparse_table st;

    rmq(size_t n, const int *data);
    ~rmq();
    int query(int l, int r);
};

#endif
