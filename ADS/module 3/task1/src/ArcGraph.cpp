#include "../include/ArcGraph.h"

void ArcGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < arcs.size());
    assert(0 <= to && to < arcs.size());
    arcs.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return count_vertices;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < arcs.size());
    std::vector<int> next_vertices;
    for (const auto &i: arcs) {
        if (i.first == vertex) {
            next_vertices.push_back(i.second);
        }
    }
    return next_vertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < arcs.size());
    std::vector<int> prev_vertices;

    for (const auto &i: arcs) {
        if (i.second == vertex) {
            prev_vertices.push_back(i.first);
        }
    }
    return prev_vertices;
}
