#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <vector>
#include <iostream>

constexpr double alpha = 0.75;

struct Node {
    long long val;
    long long sum;
    long long count;
    long long descendants;
    Node *left, *right;

    Node() : val(0), sum(0), count(0), descendants(0), left(nullptr), right(nullptr) {}
    Node(long long val_, long long sum_, long long count_, long long descendants_) : 
        val(val_), sum(sum_), count(count_), descendants(descendants_), left(nullptr), right(nullptr) {}
};
std::vector<Node> nodes;
Node *root{};
Node *rebuild_node{};
Node *rebuild_parent{};

// you can do data structure initialization in this function
void init(std::size_t n [[maybe_unused]]) {
    nodes.reserve(n);
    root = nullptr;
}

// You can deconstruct your data structure in this function
void clear() {
    nodes.clear();
}

void subtree_to_vector(Node *node, std::vector<Node*> &vec) {
    if (!node) { return; }
    subtree_to_vector(node->left, vec);
    vec.push_back(node);
    subtree_to_vector(node->right, vec);
}

void update(Node *&node){
    if (!node) { return; }
    update(node->left);
    update(node->right);
    node->descendants = node->count + (node->left ? node->left->descendants : 0) + (node->right ? node->right->descendants : 0);
    node->sum = node->count * node->val + (node->left ? node->left->sum : 0) + (node->right ? node->right->sum : 0);
}

Node *build(std::size_t l, std::size_t r, std::vector<Node*> &vec) {
    if (l > r) { return nullptr; }
    if (l == r) {
        vec[l]->left = nullptr;
        vec[l]->right = nullptr;
        return vec[l];
    }
    std::size_t mid = (l + r) >> 1;
    vec[mid]->left = (mid >= 1) ? build(l, mid - 1, vec) : nullptr;
    vec[mid]->right = build(mid + 1, r, vec);
    return vec[mid];
}

Node* rebuild(Node *&node) {
    if (!node) { return node; }
    std::vector<Node *> vec;
    subtree_to_vector(node, vec);
    node = build(0, vec.size() - 1, vec);
    update(node);
    return node;
}

void do_insert(Node *&node, Node* parent, long long val) {
    if (!node) {
        node = &nodes.emplace_back(val, val, 1, 1);
        return;
    }
    node->sum += val;
    node->descendants++;
    if (val < node->val) {
        do_insert(node->left, node, val);
    } else if (val > node->val) {
        do_insert(node->right, node, val);
    } else {
        node->count++;
    }
    if (1.0 * (node->left ? node->left->descendants : 0) > alpha * node->descendants || 1.0 *  (node->right ? node->right->descendants : 0) > alpha * node->descendants) {
        rebuild_node = node;
        rebuild_parent = parent;
    }
}

void insert(long long val) {
    rebuild_node = nullptr;
    rebuild_parent = nullptr;
    do_insert(root, nullptr, val);
    if (rebuild_parent) {
        if (rebuild_node == rebuild_parent->left) {
            rebuild_parent->left = rebuild(rebuild_node);
        } else {
            rebuild_parent->right = rebuild(rebuild_node);
        }
    } else if (rebuild_node == root) {
        rebuild(root);
    }
}

void erase(Node *node, long long val) {
    if (!node) { return; }
    node->descendants--;
    node->sum -= val;
    if (node->val == val) {
        node->count--;
    } else if (node->val > val) {
        erase(node->left, val);
    } else {
        erase(node->right, val);
    }
}

long long search(Node *node, long long x) {
    if (!node) { return 0; }
    if (node->count > 0 && node->val == x) { return 1; }
    if (node->val < x) {
        return search(node->right, x);
    } else if (node->val > x) {
        return search(node->left, x);
    }
    return 0;
}

long long search_kth(Node *node, long long k) {
    while (node) {
        long long index = node->descendants - (node->right ? node->right->descendants : 0);
        if (index < k) {
            return search_kth(node->right, k - index);
        }
        for (long long i = 0; i < node->count; i++) {
            if (index == k) { return node->val; }
            index--;
        }
        node = node->left;
    }
    return 0;
}

long long search_sum(Node *node, long long t, long long sum, long long min) {
    if (!node) { return min; }
    long long ans = (t + 1) * sum - root->sum;
    if (ans < 0) {
        sum += node->right ? node->right->sum - (node->right->right ? node->right->right->sum : 0) : 0;
        return search_sum(node->right, t, sum, min);
    } else if (ans > 0) {
        sum -= node->count * node->val + (node->left ? (node->left->right ? node->left->right->sum : 0) : 0);
        return search_sum(node->left, t, sum, std::min(ans, min));
    }
    return 0;
}

// opt==1 insert x
// opt==2 erase x
// opt==3 query x
// opt==4 query xth
// opt==5 query x*sum(s1...st) - sum(s_t+1...s_m)
long long SetOperation(std::size_t opt, long long x) {
    // insert x
    if (opt == 1) {
        insert(x);
    }

    // erase x
    else if (opt == 2) {
        erase(root, x);
    }

    // query x
    else if (opt == 3) {
        return search(root, x);
    }

    // query xth
    else if (opt == 4) {
        return search_kth(root, x);
    }

    // query x*sum(s1...st) - sum(s_t+1...s_m)
    else if (opt == 5) {
        return search_sum(root, x, root->sum - (root->right ? root->right->sum : 0), x * root->sum);
    }

    return 0;
}

#endif // CS10123F_PA2_BST