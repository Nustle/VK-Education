#ifndef LISTGRAPH_HPP
#define LISTGRAPH_HPP

#include "IGraph.h"
#include <cassert>

struct ListGraph : public IGraph {
public:
    explicit ListGraph(int size)
        : adjacency_lists(size) {}

    explicit ListGraph(const IGraph &graph)
    : adjacency_lists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            adjacency_lists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacency_lists;
};

#endif
