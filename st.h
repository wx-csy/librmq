#include <cstdint>
using std::size_t;

constexpr size_t MAXLOGN = 64;

struct sparse_table {
    size_t n;
    int *st[MAXLOGN] = {};
    int *data = nullptr;
    
    sparse_table() { n = 0; }
    sparse_table(size_t n, int *data) { init(n, data); }
    void init(size_t n, int *data, int *first = nullptr);
    ~sparse_table();
    int query(int l, int r);
};
