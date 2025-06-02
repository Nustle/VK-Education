#ifndef ARCGRAPH_H
#define ARCGRAPH_H

#include <cassert>
#include "IGraph.h"

struct ArcGraph : public IGraph {
public:
    explicit ArcGraph(int size) : count_vertices(size) {};

    explicit ArcGraph(const IGraph &graph) : count_vertices(graph.VerticesCount()) {
        for (int from = 0; from < graph.VerticesCount(); from++) {
            for (int to: graph.GetNextVertices(from)) {
                arcs.emplace_back(from, to);
            }
        }
    }

    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int count_vertices;
    std::vector<std::pair<int, int>> arcs;
};

#endif
