#include "include/Graph.hpp"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <time.h> 

//  Генерация матрицы смежности
bool IsMetric(std::vector<std::vector<int>>& graph) {
    int len = graph.size();
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            for (int s = 0; s < len; ++s) {
                if (graph[i][s] + graph[s][j] < graph[i][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}


void MakeMetric(std::vector<std::vector<int>>& graph) {
    int len = graph.size();
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            for (int s = 0; s < len; ++s) {
                if (graph[i][s] + graph[s][j] < graph[i][j]) {
                    graph[i][j] = graph[i][s] + graph[s][j];
                    graph[j][i] = graph[i][j];
                }
            }
        }
    }
}

std::vector<std::vector<int>> GenerateMatrix(int len) {
    std::vector<std::vector<int>> graph(len, std::vector<int>(len, 0));
    srand(time(NULL));
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            graph[i][j] = rand() % 50;
        }
    }
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            if (i > j) {
                graph[i][j] = graph[j][i];
            }
        }
    }

    while (!IsMetric(graph)) {
        MakeMetric(graph);
    }

    return graph;
}


// Пербор для поиска правильного ответа
int BaseAlgo(std::vector<std::vector<int>>& graph) {
    int len = graph.size();
    std::vector<int> perm;
    for (int i = 0; i < len; ++i) {
        perm.push_back(i);
    }
    int answer = 1000000000;

    do {
        int cur_ans = 0;
        for (int i = 1; i < perm.size(); ++i) {
            cur_ans += graph[perm[i - 1]][perm[i]];
        }
        cur_ans += graph[perm[len - 1]][perm[0]];
        answer = std::min(answer, cur_ans);
    } while(std::next_permutation(perm.begin(), perm.end()));

    return answer;
}

void DoTest() {
    std::vector<int> ans_algo;
    std::vector<int> ans_cur;
    std::vector<double> time_algo;
    std::vector<double> time_cur;

    for (int i = 3; i < 14; ++i) {
        int len = i;
        auto g = GenerateMatrix(len);
     
        GraphTSP graph(len);

        for (int i = 0; i < len; ++i) {
            for (int j = 0; j < len; ++j) {
                if (i != j) {
                    graph.InsertNoOrientEdge(i, j, g[i][j]);
                }
            }
        }

        time_t start_first, end_first, start_second, end_second;
        time(&start_first);
        int algo_ans = SolveTSPProblem(graph).second;
        time(&end_first);

        time(&start_second);
        int base_algo = BaseAlgo(g);
        time(&end_second);

        ans_algo.push_back(algo_ans);
        ans_cur.push_back(base_algo);

        time_algo.push_back(difftime(end_first, start_first));
        time_cur.push_back(difftime(end_second, start_second));

    }
    std::ofstream out;
    out.open("answers1.txt");
    for (int i = 0; i < ans_algo.size(); ++i) {
        out << ans_algo[i] << "\n";
    }
    out.close();
    out.open("answers2.txt");
    for (int i = 0; i < ans_cur.size(); ++i) {
        out << ans_cur[i] << "\n";
    }
    out.close();
    out.open("time1.txt");
    for (int i = 0; i < time_algo.size(); ++i) {
        out << time_algo[i] << "\n";
    }
    out.close();
    out.open("time2.txt");
    for (int i = 0; i < time_cur.size(); ++i) {
        out << time_cur[i] << "\n";
    }
    out.close();
}