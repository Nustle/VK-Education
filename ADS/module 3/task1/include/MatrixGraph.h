#ifndef MATRIXGRAPH_HPP
#define MATRIXGRAPH_HPP

#include "IGraph.h"
#include <cassert>

struct MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(size_t size) : matrix(size, std::vector<int>(size, 0)) {};

    explicit MatrixGraph(const IGraph &graph)
    : matrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0)) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            for (int to: graph.GetNextVertices(i)) {
                matrix[i][to] = 1;
            }
        }
    }

    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> matrix;
};

#endif
