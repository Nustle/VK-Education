#ifndef SETGRAPH_H
#define SETGRAPH_H

#include <set>
#include <cassert>

#include "IGraph.h"

struct SetGraph : public IGraph {
public:
    explicit SetGraph(size_t size) : hash_table_list(size) {}

    explicit SetGraph(const IGraph &graph) : hash_table_list(graph.VerticesCount()) {
        for (int from = 0; from < graph.VerticesCount(); from++) {
            for (int to: graph.GetNextVertices(from)) {
                hash_table_list[from].insert(to);
            }
        }
    }

    ~SetGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::set<int>> hash_table_list;
};

#endif 
