#include "utils.hpp"

MainGraph readGraph() {
    int vertexNumber;
    std::cout << "Введите число вершин:\n";
    std::cin >> vertexNumber;
    std::cout << "Введите матрицу смежности графа с 0 на диагонали и с -1 если ребро отсутствует.\n";
    std::cout << "Вершины будут занумерованы от 1 до " << vertexNumber << " в порядке ввода:\n";
    std::vector<std::vector<int>> graph(vertexNumber, std::vector<int>(vertexNumber));
    for (int i = 0; i < vertexNumber; ++i) {
        for (int j = 0; j < vertexNumber; ++j) {
            std::cin >> graph[i][j];
            if (graph[i][j] < 0) {
                graph[i][j] = INF;
            }
        }
    }
    return MainGraph(graph);
}

RequiredSubgraph readRequiredEdges(int totalVertexNumber) {
    int requiredEdgesNumber;
    std::cout << "Введите число ребёр, которые необходимо посетить:\n";
    std::cin >> requiredEdgesNumber;
    std::cout << "Введите необходимые для посещения ребра как пары соотвествующих вершин:\n";
    std::vector<std::pair<int, int>> inputCache(requiredEdgesNumber);
    std::vector<std::vector<int>> requiredSubgraph(totalVertexNumber);
    std::vector<int> vertexDegrees(totalVertexNumber, 0);  // count vertex degrees
    int v;
    int u;
    for (int i = 0; i < requiredEdgesNumber; ++i) {
        std::cin >> v >> u;
        --u;
        --v;
        inputCache.emplace_back(v, u);
        ++vertexDegrees[v];
        ++vertexDegrees[u];
        requiredSubgraph[v].push_back(u);
        requiredSubgraph[u].push_back(v);
    }

    auto required = RequiredSubgraph(requiredSubgraph, vertexDegrees, inputCache);
    return required;
}

int getComponentsNumber() {
    int componentNumber;
    std::cout << "Введите число компонент связности в графе G<R>:\n";
    std::cin >> componentNumber;
    return componentNumber;
}


int main() {
    // read data
    auto graph = readGraph();
    auto requiredSubgraph = readRequiredEdges(static_cast<int>(graph.GetSize()));
    int componentsNumber = getComponentsNumber();

    //find connected components on required edges
    requiredSubgraph.FindConnectedComponents();

    if (requiredSubgraph.GetComponentsNumber() != componentsNumber) {
        std::cout << "Количество компонент связности в графе на необходимых ребрах "
                     "отличается от введенного, проверьте корректность ввода\n";
        return 1;
    } else if (componentsNumber == 1) {
        std::cout << "Компонента связности графа на необходмых ребрах всего одна,"
                     " данный алгоритм не предназчен для такого случая\n";
        return 1;
    }

    // Floyd algorithm to find distance between every pair
    auto dist = graph.Floyd();
    auto degrees = requiredSubgraph.GetDegrees();
    int minimalCycleWeight = INF;
    std::vector<std::pair<int, int>> res(requiredSubgraph.GetComponentsReference().size());
    // find matching M and c-1 edge in T
    const int firstComponentIndex = 1;
    chooseVariant(requiredSubgraph.GetComponentsReference(), dist, firstComponentIndex, res, degrees, minimalCycleWeight);

    // add initial
    for (auto [u, v]: requiredSubgraph.GetInputReference()) {
        minimalCycleWeight += dist[u][v];
    }
    std::cout << "Цикл минимального веса " << minimalCycleWeight << '\n';
    return 0;
}

//0 7 9 10 10
//7 0 9 16 12
//9 9 0 8 11
//10 16 8 0 5
//10 12 11 5 0