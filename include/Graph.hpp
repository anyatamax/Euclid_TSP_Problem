#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <stack>

#include "Heap.hpp"
#include "MinimumCostPerfectMatching/Matching.h"
#include "MinimumCostPerfectMatching/Graph.h"


class GraphTSP {
public:
    explicit GraphTSP(int size) : adj_(size), dist_(size, max_distance_), prev_(size, -1){};
    std::pair<int, int> ReturnEdges(int ver_first, int ver_second) const;
    void InsertNoOrientEdge(int ver_first, int ver_second, int weight = 0);
    int GetSize();
    bool HasEdge(int ver_first, int ver_second);
    int GetWeight(int ver_first, int ver_second);
    
    // Find MST
    std::set<std::pair<int, int>> Prima();
    
    // Build graph with only odd vertex in MST
    std::pair<std::set<std::pair<int, int>>, std::set<int>> GetOddGraphTSP(GraphTSP &old);
    
    // Find minimum cost perfect matching
    std::set<std::pair<int, int>> GetMinCostPerfectMatching(std::set<std::pair<int, int>>& edges, std::set<int>& odd_ver);

    // Build euler path in mst and matching graph and get result
    std::vector<int> EulerPath();

private:
    const int max_distance_ = 100000000;
    std::vector<std::vector<int>> adj_;
    std::vector<int> dist_;
    std::vector<int> prev_;
    std::map<std::pair<int, int>, int> weights_;
    std::map<std::pair<int, int>, bool> count_each_edge_;
};


std::pair<std::vector<int>, int> SolveTSPProblem(GraphTSP& graph);