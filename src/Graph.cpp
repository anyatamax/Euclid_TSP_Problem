#include "../include/Graph.hpp"
#include <iostream>
#include <map>


int GraphTSP::GetSize() {
    return adj_.size();
}

void GraphTSP::InsertNoOrientEdge(int ver_first, int ver_second, int weight) {
    if (weights_.find(ReturnEdges(ver_first, ver_second)) == weights_.end()) {
        weights_[ReturnEdges(ver_first, ver_second)] = weight;
        count_each_edge_[ReturnEdges(ver_first, ver_second)] = true;
        adj_[ver_first].push_back(ver_second);
        adj_[ver_second].push_back(ver_first);
    } else {
        weights_[ReturnEdges(ver_first, ver_second)] = std::min(weights_[ReturnEdges(ver_first, ver_second)], weight);
    }
}

std::pair<int, int> GraphTSP::ReturnEdges(int ver_first, int ver_second) const {
    if (ver_first > ver_second) {
        return std::make_pair(ver_second, ver_first);
    }
    return std::make_pair(ver_first, ver_second);
}

bool GraphTSP::HasEdge(int ver_first, int ver_second) {
    if (count_each_edge_.find(ReturnEdges(ver_first, ver_second)) == count_each_edge_.end()) {
        return false;
    }
    return true;
}

int GraphTSP::GetWeight(int ver_first, int ver_second) {
    return weights_[ReturnEdges(ver_first, ver_second)];
}

std::set<std::pair<int, int>> GraphTSP::Prima() {
    std::set<std::pair<int, int>> weight_mst;
    dist_[0] = 0;
    std::vector<bool> used(dist_.size(), false);
    Heap h;
    for (int64_t i = 0; i < static_cast<int64_t>(adj_.size()); ++i) {
        h.Insert(std::make_pair(dist_[i], i));
    }
    while (!h.Empty()) {
        std::pair<int, int> ver = h.ExtractMin();
        if (dist_[ver.second] != ver.first) {
            continue;
        }
        if (prev_[ver.second] != -1) {
            weight_mst.insert(ReturnEdges(prev_[ver.second], ver.second));
        }
        used[ver.second] = true;
        for (auto &u : adj_[ver.second]) {
            if (!used[u] && dist_[u] > weights_[ReturnEdges(ver.second, u)]) {
                prev_[u] = ver.second;
                dist_[u] = weights_[ReturnEdges(ver.second, u)];
                h.Insert(std::make_pair(dist_[u], u));
            }
        }
    }
    
    return weight_mst;
}

std::pair<std::set<std::pair<int, int>>, std::set<int>> GraphTSP::GetOddGraphTSP(GraphTSP &old) {
    std::set<int> odd_vertex;
    for (int i = 0; i < adj_.size(); ++i) {
        if (adj_[i].size() % 2 != 0) {
            odd_vertex.insert(i);
        }
    }

    std::set<std::pair<int, int>> graph_with_odd_ver;
    for (auto &v: odd_vertex) {
        for (auto &u: odd_vertex) {
            if (old.HasEdge(v, u)) {
                graph_with_odd_ver.insert(ReturnEdges(v, u));
            }
        }
    }

    return std::make_pair(graph_with_odd_ver, odd_vertex);
}

std::set<std::pair<int, int>> GraphTSP::GetMinCostPerfectMatching(std::set<std::pair<int, int>>& edges, std::set<int>& odd_ver) {
    std::map<int, int> map_ver_to_new;
    std::map<int, int> map_ver_to_old;
    int idx = 0;
    for (auto &ver: odd_ver) {
        map_ver_to_new[ver] = idx;
        map_ver_to_old[idx] = ver;
        idx++;
    }

    Graph g(odd_ver.size());
    std::vector<double> weights(edges.size(), 0);

    for (auto &edge: edges) {
        g.AddEdge(map_ver_to_new[edge.first], map_ver_to_new[edge.second]);
        weights[g.GetEdgeIndex(map_ver_to_new[edge.first], map_ver_to_new[edge.second])] = weights_[ReturnEdges(edge.first, edge.second)];
    }

    Matching match_algo(g);
    auto result = match_algo.SolveMinimumCostPerfectMatching(weights);

    std::set<std::pair<int, int>> edges_result;
    for (auto &num_edge: result.first) {
        std::pair<int, int> edge = g.GetEdge(num_edge);
        edges_result.insert(ReturnEdges(map_ver_to_old[edge.first], map_ver_to_old[edge.second]));
    }

    return edges_result;
}

std::vector<int> GraphTSP::EulerPath() {
    int ver_start = -1;
    for (int i = 0; i < adj_.size(); ++i) {
        if (!adj_[i].empty() && ver_start == -1) {
            ver_start = i;
            break;
        }
    }

    std::vector<int> euler_path;
    std::set<int> path_ver;
    std::stack<int> s;
    s.push(ver_start);
    while(!s.empty()) {
        int ver = s.top();
        bool found_edge = false;
        for (auto &i : adj_[ver]) {
            std::pair<int, int> pair_ver = ReturnEdges(ver, i);
            if (count_each_edge_.find(pair_ver) != count_each_edge_.end() && count_each_edge_[pair_ver]) {
                s.push(i);
                count_each_edge_[pair_ver] = false;
                found_edge = true;
                break;
            }
        }
        if (!found_edge) {
            s.pop();
            if (path_ver.find(ver) == path_ver.end()) {
                euler_path.push_back(ver);
            }
            path_ver.insert(ver);
        }
    }

    return euler_path;
}


std::pair<std::vector<int>, int> SolveTSPProblem(GraphTSP& graph) {
    auto mst_graph_edges = graph.Prima();
    GraphTSP mst_graph(graph.GetSize());
    for (auto &edge: mst_graph_edges) {
        mst_graph.InsertNoOrientEdge(edge.first, edge.second, graph.GetWeight(edge.first, edge.second));
    }

    auto graph_with_odd_vertex = mst_graph.GetOddGraphTSP(graph);

    auto matching_graph = graph.GetMinCostPerfectMatching(graph_with_odd_vertex.first, graph_with_odd_vertex.second);

    GraphTSP join_graph(graph.GetSize());
    for (auto &edge: mst_graph_edges) {
        if (!join_graph.HasEdge(edge.first, edge.second)) {
            join_graph.InsertNoOrientEdge(edge.first, edge.second, 
                graph.GetWeight(edge.first, edge.second));
        }
    }
    for (auto &edge: matching_graph) {
        if (!join_graph.HasEdge(edge.first, edge.second)) {
            join_graph.InsertNoOrientEdge(edge.first, edge.second, 
                graph.GetWeight(edge.first, edge.second));
        }
    }

    auto path_result = join_graph.EulerPath();
    path_result.push_back(path_result[0]);
    int path_cost = 0;
    for (int i = 1; i < path_result.size(); ++i) {
        path_cost += graph.GetWeight(path_result[i - 1], path_result[i]);
    }

    return std::make_pair(path_result, path_cost);
}