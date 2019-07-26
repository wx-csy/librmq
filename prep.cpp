#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <stack>
#include <utility>
#include <algorithm>
#include <numeric>
#include <bitset>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstddef>

constexpr size_t SEG_LENGTH = 8;

typedef std::array<uint8_t, SEG_LENGTH> perm_t, preorder_t;

std::vector<preorder_t> trees;
std::map<preorder_t, int> tree_id;
std::map<preorder_t, perm_t> tree_sample;

struct instance {
    std::bitset<SEG_LENGTH * SEG_LENGTH> bs;
    int perm_id, tree_id;

    instance(perm_t perm, int perm_id, int tree_id) :
            perm_id(perm_id), tree_id(tree_id) {
        std::memset(&bs, 0, sizeof bs);
        for (int i = 0; i < SEG_LENGTH; i++)
            for (int j = 0; j < i; j++)
                bs[j * SEG_LENGTH + i] = (perm[j] < perm[i]);
    }
};

int next_node_id = 0;
struct decision_tree_node {
    int id, crit, tree_id;
    decision_tree_node *subtree[2] = {};
    
    static int should_stop(const std::vector<instance>& instances) {
        int tree_id = instances.front().tree_id;
        for (auto& inst : instances) 
            if (inst.tree_id != tree_id) return -1;
        return tree_id;
    }

    static int select_criterion(const std::vector<instance>& instances) {
        // balance rule
        int cnt[SEG_LENGTH * SEG_LENGTH] = {0};
        for (int i = 0; i < SEG_LENGTH * SEG_LENGTH; i++) {
            for (auto& inst : instances) if (inst.bs[i]) cnt[i]++;
            cnt[i] = std::min(cnt[i], (int)instances.size() - cnt[i]);
        }
        return std::max_element(std::begin(cnt), std::end(cnt)) - cnt;
    }

    decision_tree_node(const std::vector<instance>& instances, int dep) {
        id = next_node_id++;
        tree_id = should_stop(instances);
        if (tree_id >= 0) {
            crit = -1;
        } else {
            crit = select_criterion(instances);
            std::vector<instance> subtree_inst[2];
            for (auto& inst : instances) 
                subtree_inst[inst.bs[crit]].push_back(inst);
            for (int i = 0; i < 2; i++)
                subtree[i] = new decision_tree_node(subtree_inst[i], dep + 1);
        }
    }
    
    void serialize() {
        if (crit < 0) {
            printf("    {-1, %d, -1, -1},\n", tree_id);
        } else {
            printf("    {%d, -1, %d, %d},\n", 
                crit, subtree[0]->id, subtree[1]->id);
        }
        if (subtree[0]) subtree[0]->serialize();
        if (subtree[1]) subtree[1]->serialize();
    }

    ~decision_tree_node() {
        delete subtree[0];
        delete subtree[1];
    }
};

std::vector<instance> instances;

preorder_t perm_to_preorder(perm_t perm) {
    preorder_t result;
    int ptr = 0;
    std::stack<std::pair<int, int>> stk; stk.emplace(0, SEG_LENGTH);
    while (stk.size()) {
        int l, r; std::tie(l, r) = stk.top(); stk.pop();
        if (l == r) continue;
        int m = std::min_element(perm.begin() + l, perm.begin() + r) - perm.begin();
        result[ptr++] = m;
        stk.emplace(l, m);
        stk.emplace(m + 1, r);
    }
    return result;
}

void print_lookup(perm_t perm) {
    int result[SEG_LENGTH][SEG_LENGTH];
    memset(result, -1, sizeof result);
    for (int i = 1; i <= SEG_LENGTH; i++) {
        for (int j = 0; j < i; j++) {
            result[j][i-1] = std::min_element(perm.begin() + j, perm.begin() + i) - perm.begin();
        }
    }
    putchar('{');
    for (int i = 0; i < SEG_LENGTH; i++)
        for (int j = 0; j < SEG_LENGTH; j++)
            printf("%d,", result[i][j]);
    printf("},\n");
}

void print_lookup_pref(perm_t perm) {
    putchar('{');
    for (int i = 1; i <= SEG_LENGTH; i++) {
        int value = std::min_element(perm.begin(), perm.begin() + i) - perm.begin(); 
        printf("%d,", value);
    }
    printf("},\n");
}

void print_lookup_suf(perm_t perm) {
    putchar('{');
    for (int i = 0; i < SEG_LENGTH; i++) {
        int value = std::min_element(perm.begin() + i, perm.end()) - perm.begin(); 
        printf("%d,", value);
    }
    printf("},\n");
}

void work() {
    perm_t perm; std::iota(perm.begin(), perm.end(), 0);
    do {
        preorder_t preorder = perm_to_preorder(perm);
        int id = tree_id.emplace(preorder, tree_id.size()).first->second;
        if (id == trees.size()) {
            trees.emplace_back(preorder);
            tree_sample.emplace(preorder, perm);
        }
        instances.emplace_back(perm, instances.size(), id);
    } while (std::next_permutation(perm.begin(), perm.end()));
}

int main() {
    work();
    decision_tree_node *node = new decision_tree_node(instances, 0);
    puts("#include <cstdint>");
    puts("#include \"hardcode.h\"");
    puts("namespace librmq {");
    puts("static const decision_tree_node _nodes[] = {");
    node->serialize();
    delete node;
    puts("};\n");
    fprintf(stderr, "# of nodes in decision tree: %d\n", next_node_id);
    puts("decision_tree_node const * const decision_tree_nodes = _nodes;");

    puts("const std::int8_t lookup_table[][SEGLEN][SEGLEN] = {");
    for (const auto& tr : trees) print_lookup(tree_sample[tr]);
    puts("};\n");
    
    puts("const std::int8_t lookup_pref[][SEGLEN] = {");
    for (const auto& tr : trees) print_lookup_pref(tree_sample[tr]);
    puts("};\n");
    
    puts("const std::int8_t lookup_suf[][SEGLEN] = {");
    for (const auto& tr : trees) print_lookup_suf(tree_sample[tr]);
    puts("};\n");
    
    puts("}");
    fprintf(stderr, "# of different Cartesian trees: %d\n", (int)trees.size());
    return 0;
}

