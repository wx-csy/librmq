#include <numeric>
#include "st.h"

void sparse_table::init(const size_t n, int * const data, int * const first) {
    this->n = n; this->data = data;
    if (first) {
        st[0] = first; 
    } else { 
        st[0] = new int[n]; std::iota(st[0], st[0] + n, 0);
    }
    for (int level = 1; (1 << level) <= n; level++) {
        st[level] = new int[n - (1 << level) + 1];
        for (int i = 0; i + (1 << level) <= n; i++) {
            int p1 = st[level - 1][i], p2 = st[level - 1][i + (1 << level)];
            st[level][i] = (data[p1] < data[p2] ? p1 : p2);
        }
    }
}

sparse_table::~sparse_table() {
    for (int *st_entry : st) delete[] st_entry;
}

static inline int fast_log2(int length) {
    return sizeof(int) * 8 - 1 - __builtin_clz(length);
}

int sparse_table::query(int l, int r) {
    if (l >= r) return -1;
    int level = fast_log2(r - l);
    int p1 = st[level][l], p2 = st[level][r - (1 << level)];
    return data[p1] < data[p2] ? p2 : p1; 
}
