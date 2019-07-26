#ifndef __ST_H__
#define __ST_H__

#include <cstdint>
using std::size_t;

constexpr size_t MAXLOGN = 64;

struct sparse_table {
    size_t n;
    int *st[MAXLOGN] = {};
    const int *data = nullptr;
    
    sparse_table() { n = 0; }
    sparse_table(size_t n, const int *data) { init(n, data); }
    void init(size_t n, const int *data, int *first = nullptr);
    ~sparse_table();
    int query(int l, int r);
};

#endif
