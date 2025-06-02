#include "../include/SetGraph.h"

void SetGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < hash_table_list.size());
    assert(0 <= to && to < hash_table_list.size());
    hash_table_list[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(hash_table_list.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < hash_table_list.size());
    std::vector<int> next_vertices;
    for (int i: hash_table_list[vertex]) {
        next_vertices.push_back(i);
    }
    return next_vertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < hash_table_list.size());
    std::vector<int> prev_vertices;

    for (int from = 0; from < hash_table_list.size(); from++) {
        for (int to: hash_table_list[from]) {
            if (to == vertex) {
                prev_vertices.push_back(from);
            }
        }
    }
    return prev_vertices;
}