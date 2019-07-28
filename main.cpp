#include <cstdlib>
#include <utility>
#include <random>
#include <cstdio>
#include <tuple>
#include <array>
#include <chrono>
#include "librmq.h"
using namespace librmq;
std::mt19937 gen(23529432);

template <typename TSolver>
void functional_test_instance(int size) {
    static int epoch = 0;
    printf("Running epoch #%d (size=%d) ... ", epoch++, size);
    int *data = new int[size];
    for (int i = 0; i < size; i++) data[i] = gen();
    rmq_st<int> st(size, data);
    TSolver rmqr(size, data);
    for (int i = 0; i < size; i++) {
        int l = gen() % size, r = gen() % size + 1;
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

template <template<typename> typename TSolver>
void functional_test() {
    for (int i = 0; i < 100; i++) 
        functional_test_instance<TSolver<int>>(gen() % 10 + 1);
    for (int i = 0; i < 100; i++) 
        functional_test_instance<TSolver<int>>(gen() % 50 + 1);
    for (int i = 0; i < 20; i++) 
        functional_test_instance<TSolver<int>>(gen() % 100000 + 1);
    for (int i = 0; i < 10; i++) 
        functional_test_instance<TSolver<int>>(gen() % 1000000 + 1);
}

template <typename TSolver>
std::pair<double, double> performance_test_instance(size_t size, size_t nq, int seed = 123456) {
    std::mt19937 gen(seed);
    int *data = new int[size];
    size_t *ql = new size_t[nq], *qr = new size_t[nq];
    for (int i = 0; i < size; i++) data[i] = gen();
    
    using namespace std::chrono;
    double tprep, tq;
    
    auto tstart = high_resolution_clock::now(), tend = tstart;
    TSolver solver(size, data);
    tend = high_resolution_clock::now();
    tprep = duration_cast<duration<double>>(tend - tstart).count();
    
    for (size_t i = 0; i < nq; i++) {
        do {
            ql[i] = gen() % size; qr[i] = gen() % size + 1;
        } while (ql[i] == qr[i]);
        if (ql[i] > qr[i]) std::swap(ql[i], qr[i]);
    }
    
    volatile unsigned checksum = 0;
    tstart = high_resolution_clock::now();
    for (size_t i = 0; i < nq; i++) checksum += solver.query(ql[i], qr[i]); 
    tend = high_resolution_clock::now();
    tq = duration_cast<duration<double>>(tend - tstart).count();

    delete[] data;
    delete[] ql; delete[] qr;

    return {tprep, tq};
}

template <template<typename> typename TSolver>
void performance_test(size_t size, size_t nq, size_t rep = 10) {
    std::vector<double> tprep, tq;
    printf("Testing size = %zu nq = %zu with %zu repititions\n", size, nq, rep);
    for (size_t i = 0; i < rep; i++) {
        printf("Round %zu ... ", i); 
        double ttprep, ttq;
        std::tie(ttprep, ttq) = performance_test_instance<TSolver<int>>(size, nq, gen());
        printf("tprep = %.6fs, tquery = %.6fs\n", ttprep, ttq);
        tprep.push_back(ttprep); tq.push_back(ttq);
    }
    double atprep, atq;
    atprep = std::accumulate(tprep.begin(), tprep.end(), 0.0) / rep;
    atq = std::accumulate(tq.begin(), tq.end(), 0.0) / rep;
    printf("tprep = %.6fs, tquery = %.6fs, MQPS = %.6f, ctp/ctq = %.6f\n", 
        atprep, atq, nq / 1000000.0 / atq, atprep / atq / size * nq);
}

int main() {
    functional_test<rmq_zkw>();
    functional_test<rmq_ind>();
    functional_test<rmq_block>();
    performance_test<rmq_zkw>(10000000, 10000000, 10);
    performance_test<rmq_st>(10000000, 10000000, 10);
    performance_test<rmq_ind>(10000000, 10000000, 10);
    performance_test<rmq_block>(10000000, 10000000, 10);
    return 0;
}
