#include "st.h"

struct rmq {
    size_t n, nr_blocks;
    int *data;
    int *segid;
    sparse_table st;

    rmq(size_t n, int *data);
    ~rmq();
    int query(int l, int r);
};
