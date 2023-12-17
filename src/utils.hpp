#include <iostream>
#include <vector>
#include <unordered_set>

#define INF 100000007


int kun(const std::vector<std::vector<int>> &gr, const std::vector<int> &degrees) {
    std::vector<std::vector<int>> a(gr.size() + 1, std::vector<int>(gr.size() + 1));
    for (int i = 1; i < a.size(); ++i) {
        for (int j = 1; j < a.size(); ++j) {
            if (i == j) a[i][j] = INF;
            else a[i][j] = ((((degrees[i - 1] % 2) == 0) || ((degrees[j - 1] % 2) == 0)) ? INF : gr[i - 1][j - 1]);
        }
    }

    int n, m;
    n = m = a.size();
    std::vector<int> u(n), v(m), p(m), way(m);
    for (int i = 1; i < n; ++i) {
        p[0] = i;
        int j0 = 0;
        std::vector<int> minv(m, INF + 1);
        std::vector<char> used(m, false);
        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF + 1, j1;
            for (int j = 1; j < m; ++j)
                if (!used[j]) {
                    int cur = a[i0][j] - u[i0] - v[j];
                    if (cur < minv[j])
                        minv[j] = cur, way[j] = j0;
                    if (minv[j] < delta)
                        delta = minv[j], j1 = j;
                    used[j0] = true;
                }
            for (int j = 0; j < m; ++j)
                if (used[j])
                    u[p[j]] += delta, v[j] -= delta;
                else
                    minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    int ans = 0;
    for (int j = 1; j < m; ++j)
        ans += (a[p[j]][j] == INF ? 0 : a[p[j]][j]);
    return ans / 2;
}

void chooseVariant(const std::vector<std::vector<int>> &components, const std::vector<std::vector<int>> &gr, int firstComponentIndex,
                   std::vector<std::pair<int, int>> res, std::vector<int>& degrees, int &ans) {

    if (firstComponentIndex == components.size()) {
        int cur = 0;
        for (auto [u, v]: res)
            cur += gr[u][v];
        cur += kun(gr, degrees);
        ans = std::min(ans, cur);
        return;
    }
    for (int firstComponentVertexIndex = 0; firstComponentVertexIndex < components[firstComponentIndex].size(); ++firstComponentVertexIndex) {
        for (int secondComponentIndex = 0; secondComponentIndex < firstComponentIndex; ++secondComponentIndex) {
            for (int secondComponentVertexIndex = 0; secondComponentVertexIndex < components[secondComponentIndex].size(); ++secondComponentVertexIndex) {

                int firstComponentVertex = components[firstComponentIndex][firstComponentVertexIndex];
                int secondComponentVertex = components[secondComponentIndex][secondComponentVertexIndex];

                res[firstComponentIndex] = {firstComponentVertex, secondComponentVertex};

                ++degrees[firstComponentVertex];
                ++degrees[secondComponentVertex];

                chooseVariant(components, gr, firstComponentIndex + 1, res, degrees, ans);

                --degrees[firstComponentVertex];
                --degrees[secondComponentVertex];
            }
        }
    }
}



class MainGraph {
public:
    explicit MainGraph(std::vector<std::vector<int>> graph) : graph(graph), dist(std::move(graph)) {}

    size_t GetSize() {
        return graph.size();
    }

    const std::vector<std::vector<int>> &Floyd() {
        size_t n = dist.size();
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
        return dist;
    }

private:

    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> dist;
};


class RequiredSubgraph {
public:

    explicit RequiredSubgraph(std::vector<std::vector<int>> graph, std::vector<int> degrees, std::vector<std::pair<int, int>> input)
            : used(graph.size(), false), graph(std::move(graph)), degrees(std::move(degrees)), inputCache(std::move(input))
            {}

    void FindConnectedComponents() {
        int cnt = 0;
        for (int i = 0; i < graph.size(); ++i) {
            if (!IsUsed(i) && !graph[i].empty()) {
                DFS(i, cnt++);
            }
        }
    }

    std::vector<std::vector<int>> GetConnectedComponents() const {
        return components;
    }

    std::vector<int> GetDegrees() const {
        return degrees;
    }

    const std::vector<std::pair<int, int>>& GetInputReference() {
        return inputCache;
    }
    std::vector<std::vector<int>> &GetComponentsReference() {
        return components;
    }

    bool IsUsed(int n) {
        return used[n];
    }

    size_t GetComponentsNumber() const {
        return components.size();
    }


private:
    void DFS(int v, int c) {
        used[v] = true;
        for (int u: graph[v]) {

            if (used[u]) {
                continue;
            }
            DFS(u, c);
        }
        used[v] = true;
        if (components.size() == c) {
            components.emplace_back();
        }
        components[c].push_back(v);
    }

private:

    std::vector<bool> used;
    std::vector<std::vector<int>> graph;
    std::vector<int> degrees;
    std::vector<std::pair<int, int>> inputCache;
    std::vector<std::vector<int>> components;
};
