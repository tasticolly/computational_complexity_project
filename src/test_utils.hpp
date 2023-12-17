#include <iostream>
#include <vector>

#define INF 100000007

std::vector<std::vector<int>> createRandomGraph(int n, int p) {
    int max_dist = 10000;
    std::vector<std::vector<int>> res(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            int r = rand() % max_dist;
            res[i][j] = (r < max_dist * p / 100) ? INF : r;
            res[j][i] = res[i][j];
        }
    }
    return res;
}

std::vector<std::vector<std::pair<int, int>>> createComponents(int c, std::vector<std::vector<int>> &gr) {
    std::vector<std::vector<std::pair<int, int>>> res(c);
    std::vector<int> vertex_to_comp(gr.size(), -1);
    int count = 0;
    for (int i = 0; i < gr.size() * 100; ++i) {
        int v = rand() % gr.size();
        int u = rand() % gr.size();
        if (u != v && gr[u][v] < INF && vertex_to_comp[u] == -1 && vertex_to_comp[v] == -1) {
            res[count].emplace_back(u, v);
            vertex_to_comp[u] = count;
            vertex_to_comp[v] = count;
            ++count;
            if (count == c) break;
        }
    }

    for (int j = 0; j < gr.size() * 100; ++j) {
        int v = rand() % gr.size();
        int u = rand() % gr.size();
        if (u != v && gr[u][v] < INF && ((vertex_to_comp[u] == -1) ^ (vertex_to_comp[v] == -1))) {
            int comp = std::max(vertex_to_comp[u], vertex_to_comp[v]);
            res[comp].emplace_back(u, v);
            vertex_to_comp[u] = comp;
            vertex_to_comp[v] = comp;
        }
    }

    return res;
}