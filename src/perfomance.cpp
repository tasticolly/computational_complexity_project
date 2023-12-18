#include "test_utils.hpp"
#include <benchmark/benchmark.h>

static void test(benchmark::State& state) {
    for (auto _ : state) {
        srand(time(0));

        int n = state.range(0);
        int p = state.range(1);
        auto graph = MainGraph(createRandomGraph(n, p));
        int c = state.range(2);
        auto edgeComponents = createEdgeComponents(c, graph);
        auto vertexComponents = getVertexComponents(edgeComponents);
        // Floyd algorithm to find dist between every pair
        auto dist = graph.Floyd();
        // count vertex degrees
        auto degrees = getDegrees(graph.GetSize(), edgeComponents);
        // find (c-1)
        int ans = INF;
        std::vector<std::pair<int, int>> res(vertexComponents.size());
        const int firstComponentIndex = 1;
        chooseVariant(vertexComponents, dist, 1, res, degrees, ans);
        // add initial
        for (const auto &c: edgeComponents) {
            for (auto [u, v]: c) {
                ans += dist[u][v];
            }
        }
    }
}

BENCHMARK(test)
->ArgsProduct({benchmark::CreateDenseRange(8, 20, 1), benchmark::CreateDenseRange(10, 90, 10), benchmark::CreateDenseRange(2, 4, 1)})
->Threads(8)
->Repetitions(1);