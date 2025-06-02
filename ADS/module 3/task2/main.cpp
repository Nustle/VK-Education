/** 2. Количество различных путей
 * Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между
 * какими-то вершинами. Найдите количество различных кратчайших путей между заданными вершинами.
 * Требуемая сложность O(V+E).
 * **/

#include <vector>
#include <cassert>
#include <iostream>
#include <queue>

using namespace std;

struct ListGraph {
public:
    explicit ListGraph(int size)
            : adjacency_lists(size) {
    }

    ~ListGraph() = default;

    void AddEdge(int from, int to) {
        assert(0 <= from && from < adjacency_lists.size());
        assert(0 <= to && to < adjacency_lists.size());
        adjacency_lists[from].push_back(to);
        adjacency_lists[to].push_back(from);
    }

    int VerticesCount() const {
        return static_cast<int>(adjacency_lists.size());
    }

    vector<int> GetNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacency_lists.size());
        return adjacency_lists[vertex];
    }

private:
    vector<vector<int>> adjacency_lists;
};

int findCountOfShortestDist(const ListGraph &graph, int start_vertex, int end_vertex, vector<bool> &visited) {
    vector<int> dist(graph.VerticesCount(), graph.VerticesCount());
    vector<int> min_paths(graph.VerticesCount(), 0);

    dist[start_vertex] = 0;
    min_paths[start_vertex] = 1;
    visited[start_vertex] = true;

    queue<int> q;
    q.push(start_vertex);

    while (!q.empty()) {
        int current_vertex = q.front();
        q.pop();

        for (int next_vertex : graph.GetNextVertices(current_vertex)) {
            if (!visited[next_vertex]) {
                q.push(next_vertex);
                visited[next_vertex] = true;
            }
            if (dist[next_vertex] == dist[current_vertex] + 1) {
                min_paths[next_vertex] += min_paths[current_vertex];
            } else if (dist[next_vertex] > dist[current_vertex] + 1) {
                dist[next_vertex] = dist[current_vertex] + 1;
                min_paths[next_vertex] = min_paths[current_vertex];
            }
        }
    }
    return min_paths[end_vertex];
}

int main() {
    int from, to, start_target, end_target;
    int vertex_count, edges_count = 0;
    cin >> vertex_count >> edges_count;

    ListGraph graph(vertex_count);
    for (int i = 0; i < edges_count; i++) {
        cin >> from >> to;
        graph.AddEdge(from, to);
    }

    vector<bool> visited(graph.VerticesCount(), false);
    cin >> start_target >> end_target;

    int ans = findCountOfShortestDist(graph, start_target, end_target, visited);
    cout << ans << endl;
}
