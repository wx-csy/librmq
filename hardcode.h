#ifndef __HARDCODE_H__
#define __HARDCODE_H__

#include <cstdint>

constexpr int SEGLEN = 8;

extern struct node {
    int crit, tree, son[2];
} const * const nodes;

extern struct prep {
    std::int8_t result[SEGLEN][SEGLEN];
} const * const lookup;

#endif
