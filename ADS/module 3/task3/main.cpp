/** 3. Города
 * Требуется отыскать самый короткий маршрут между городами. Из города может выходить дорога,
 * которая возвращается в этот же город.
 * Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
 * Первая строка содержит число N – количество городов, 3 ≤ N ≤ 10000.
 * Вторая строка содержит число M - количество дорог, 1 ≤ M ≤ 250000.
 * Каждая следующая строка содержит три числа cur_vertex, next_vertex, next_time – описание дороги: откуда, куда, время в пути. 
 * 0 ≤ cur_vertex < N, 0 ≤ next_vertex < N, 1 ≤ next_time ≤ 10000. Между любыми двумя городами может быть больше одной дороги.
 * Последняя строка содержит два числа: s и t – маршрут (откуда и куда нужно доехать). 0 ≤ s < N, 0 ≤ t < N.
 * **/

#include <vector>
#include <iostream>
#include <set>
#include <limits>

using namespace std;

struct Vertex {
    Vertex(int num, int time) : num(num), time(time) {}

    int num;
    int time;
};

struct VertexCompare {
    bool operator()(const Vertex &a, const Vertex &b) const {
        if (a.time != b.time)
            return a.time < b.time;
        return a.num < b.num;
    }
};

struct ListGraph {
public:
    explicit ListGraph(int size)
    : adjacency_lists(size) {}

    ~ListGraph() = default;

    void AddEdge(int from, int to, int weight) {
        adjacency_lists[from].emplace_back(to, weight);
        adjacency_lists[to].emplace_back(from, weight);
    }

    int VerticesCount() const {
        return static_cast<int>(adjacency_lists.size());
    }

    const vector<Vertex> &GetNextVertices(int vertex) const {
        return adjacency_lists[vertex];
    }

private:
    vector<vector<Vertex>> adjacency_lists;
};

int Dijkstra(const ListGraph &graph, int start_index, int end_index) {
    int n = graph.VerticesCount();
    const int int_max = numeric_limits<int>::max();

    vector<int> dist(n, int_max);

    set<Vertex, VertexCompare> q;

    dist[start_index] = 0;
    q.insert(Vertex(start_index, 0));

    while (!q.empty()) {
        Vertex current = *q.begin();
        q.erase(q.begin());

        int cur_vertex = current.num;
        int cur_time = current.time;
        if (cur_time > dist[cur_vertex]) {
            continue;
        }

        for (const Vertex &edge : graph.GetNextVertices(cur_vertex)) {
            int next_vertex = edge.num;
            int next_time = edge.time;
            if (dist[next_vertex] > dist[cur_vertex] + next_time) {
                if (dist[next_vertex] != int_max) {
                    q.erase(Vertex(next_vertex, dist[next_vertex]));
                }
                dist[next_vertex] = dist[cur_vertex] + next_time;
                q.insert(Vertex(next_vertex, dist[next_vertex]));
            }
        }
    }

    return dist[end_index];
}

int main() {
    int vertex_count, edges_count, from, to, weight;
    cin >> vertex_count >> edges_count;
    ListGraph graph(vertex_count);

    for (int i = 0; i < edges_count; i++) {
        cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    int start_index, end_index;
    cin >> start_index >> end_index;

    int answer = Dijkstra(graph, start_index, end_index);
    cout << answer << endl;

    return 0;
}