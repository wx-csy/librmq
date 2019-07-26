#include <cstdlib>
#include <utility>
#include <random>
#include <cstdio>
#include "st.h"
#include "rmq.h"

std::mt19937 gen(23529432);

void functional_test_instance(int size) {
    static int epoch = 0;
    printf("Running epoch #%d (size=%d) ... ", epoch++, size);
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

void functional_test() {
    for (int i = 0; i < 100; i++) functional_test_instance(gen() % 10 + 1);
    for (int i = 0; i < 100; i++) functional_test_instance(gen() % 50 + 1);
    for (int i = 0; i < 20; i++) functional_test_instance(gen() % 100000 + 1);
    for (int i = 0; i < 10; i++) functional_test_instance(gen() % 1000000 + 1);
}

int main() {
    functional_test();
    return 0;
}
