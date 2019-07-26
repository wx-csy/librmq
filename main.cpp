#include <cstdlib>
#include <utility>
#include <random>
#include <cstdio>
#include "st.h"
#include "rmq.h"

void functional_test(int seed = 123456) {
    std::mt19937 gen(seed);
    const int nr_epoch = 100;
    for (int epoch = 0; epoch < nr_epoch; epoch++) {
        int size = gen() % 100000 + 1;
        printf("Running epoch #%d (size=%d) ... ", epoch, size);
        int *data = new int[size];
        for (int i = 0; i < size; i++) data[i] = gen();
        sparse_table st(size, data);
        rmq rmqr(size, data);
        for (int i = 0; i < size; i++) {
            int l = gen() % size, r = gen() % size;
            if (l == r) continue;
            if (l > r) std::swap(l, r);
            int res_st = st.query(l, r), res_rmq = rmqr.query(l, r);
            if (data[res_st] != data[res_rmq]) {
                puts("FAIL!");
                std::printf(
                    "on query #%d (%d, %d): expected %d(%d), returned %d(%d)\n", 
                    i, l, r, data[res_st], res_st, data[res_rmq], res_rmq);
                std::exit(EXIT_FAILURE);
            }
        }
        puts("OK!");
        delete[] data;
    }
}

int main() {
    functional_test();
    return 0;
}
