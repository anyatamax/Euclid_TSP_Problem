#include <iostream>
#include "include/Graph.hpp"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <time.h> 


int main() {
    int n, m;
    std::cin >> n >> m;

    GraphTSP graph(n);

    for (int i = 0; i < m; ++i) {
        int ver_first, ver_second, weight;
        std::cin >> ver_first >> ver_second >> weight;
        graph.InsertNoOrientEdge(--ver_first, --ver_second, weight);
    }

    auto result = SolveTSPProblem(graph);
    std::cout << "Cost of path: " << result.second << "\n";
    std::cout << "Path: \n";
    for (int i = 0; i < result.first.size(); ++i) {
        std::cout << result.first[i] + 1;
    }
}