#ifndef __HARDCODE_H__
#define __HARDCODE_H__

#include <cstdint>

namespace librmq {

constexpr int SEGLEN = 8;
constexpr int NR_TREES = 1430;

extern struct decision_tree_node {
    int crit, tree, son[2];
} const * const decision_tree_nodes;

extern const std::int8_t lookup_table[NR_TREES][SEGLEN][SEGLEN];
extern const std::int8_t lookup_pref[NR_TREES][SEGLEN], lookup_suf[NR_TREES][SEGLEN];

}
#endif
