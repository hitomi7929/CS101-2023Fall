#include <iostream>
#include <vector>
#include <algorithm>


// The structure to represent the input vectors.
struct LinearVector {
    int l, r, c;
};

// Used in the sort function. (small -> large)
bool compare(LinearVector &x, LinearVector &y) {
    return x.c < y.c;
}

int find(std::vector<int> &vec, int i) {
    if (vec[i] == i) { return i; }
    vec[i] = find(vec, vec[i]);
    return vec[i];
}

void set_union(std::vector<int> &vec, int find_i, int find_j) {
    vec[find_j] = find_i;
}

int main() {
    int m, n;
    std::cin >> m >> n;
    std::vector<LinearVector> vectors(m);   // Store the input vectors.
    for (int i = 0; i < m; i++) {
        std::cin >> vectors[i].l >> vectors[i].r >> vectors[i].c;
        vectors[i].l--;
    }
    std::sort(vectors.begin(), vectors.end(), compare);

    std::vector<int> visited_list(n+1);
    for (int i = 0; i <= n; i++) {
        visited_list[i] = i;
    }

    long long ans = 0;
    int visited_count = 0;
    while (visited_count < n && !vectors.empty()) {
        LinearVector cur = vectors.back();
        vectors.pop_back();
        int left_set = find(visited_list, cur.l);
        int right_set = find(visited_list, cur.r);
        if (left_set != right_set) {
            ans += cur.c;
            visited_count++;
            set_union(visited_list, left_set, right_set);
        }
    }
    
    std::cout << ans << std::endl;
    
    return 0;
}