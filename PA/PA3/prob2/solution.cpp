#include <iostream>
#include <vector>


int main() {
    int n, m, i, j;
    std::cin >> n >> m;

    std::vector<int> V(n+2);
    std::vector<int> C(n+2);
    for (int i = 1; i <= n; i++) {
        std::cin >> V[i] >> C[i];
    }

    std::vector<std::vector<long long>> pos(n+2, std::vector<long long> (m+2));
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            if (j < C[i]) {
                pos[i][j] = pos[i-1][j];
            } else {
                pos[i][j] = std::max(pos[i-1][j], pos[i-1][j-C[i]] + V[i]);
            }
        }
    }

    std::vector<std::vector<long long>> neg(n+2, std::vector<long long> (m+2));
    for (i = n; i > 0; i--) {
        for (j = 1; j <= m; j++) {
            if (j < C[i]) {
                neg[i][j] = neg[i+1][j];
            } else {
                neg[i][j] = std::max(neg[i+1][j], neg[i+1][j-C[i]] + V[i]);
            }
        }
    }

    int p, q;
    long long max;
    long long MSV = pos[n][m];
    std::vector<long long> P;
    for (i = 1, j = m; i <= n; i++) {
        if (j >= C[i] && neg[i][j] == neg[i+1][j-C[i]] + V[i]) {
            j -= C[i];
            std::cout << i << " ";
            max = 0;
            p = i - 1;
            q = i + 1;
            for (int t = 0; t <= m && max < MSV; t++) {
                max = std::max(pos[p][t] + neg[q][m-t], max);
            }
            P.push_back((long long)V[i] + max - MSV);
        }
    }
    std::cout << std::endl;
    for (i = 0; i < (int)P.size(); i++) {
        std::cout << P[i] << " ";
    }

    return 0;
}