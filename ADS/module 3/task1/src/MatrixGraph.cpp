#include "../include/MatrixGraph.h"

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < matrix.size());
    std::vector<int> next_vertices;
    for (int to = 0; to < matrix[vertex].size(); to++) {
        if (matrix[vertex][to] == 1) {
            next_vertices.push_back(to);
        }
    }
    return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < matrix.size());
    std::vector<int> prev_vertices;

    for (int from = 0; from < matrix.size(); from++) {
        if (matrix[from][vertex] == 1) {
            prev_vertices.push_back(from);
        }
    }
    return prev_vertices;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(matrix.size());
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < matrix.size());
    assert(0 <= to && to < matrix.size());
    matrix[from][to] = 1;
}