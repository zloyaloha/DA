#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using Edge = std::pair<unsigned long long, int>;
using Vertices = std::vector<Edge>;

class Graph {
    std::vector<Vertices> _graph;
    int _amount;
    public:
        Graph(const std::vector<Vertices>& graph, int amount);
        unsigned long long Dijkstra(int start, int finish);
};

Graph::Graph(const std::vector<Vertices>& graph, int amount) : _graph(graph), _amount(amount) {}

unsigned long long Graph::Dijkstra(int start, int finish) {
    std::vector<unsigned long long> distFromStartToAll(_amount + 1, UINT64_MAX);
    distFromStartToAll[start] = 0;
    std::priority_queue<Edge, Vertices, std::greater<>> pq;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [dist, vertex] = pq.top();
        pq.pop();
        if (dist > distFromStartToAll[vertex]) {
            continue;
        }
        for (const Edge& edge: _graph[vertex]) {
            long long weight = edge.first;
            int to = edge.second;
            if (distFromStartToAll[vertex] + weight < distFromStartToAll[to]) {
                distFromStartToAll[to] = distFromStartToAll[vertex] + weight;
                pq.push({distFromStartToAll[to], to});
            }
        }
    }
    return distFromStartToAll[finish];
}

int main() {
    int n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    std::vector<Vertices> graph(n + 1);
    long long vertex1, vertex2, weight;
    for (int i = 0; i < m; ++i) {
        std::cin >> vertex1 >> vertex2 >> weight;
        graph[vertex1].emplace_back(std::make_pair(weight, vertex2));
        graph[vertex2].emplace_back(std::make_pair(weight, vertex1));
    }
    Graph gr(graph, n);
    unsigned long long solution = gr.Dijkstra(start, finish);
    if (solution == UINT64_MAX) {
        std::cout << "No solution";
    } else {
        std::cout << solution;
    }
}