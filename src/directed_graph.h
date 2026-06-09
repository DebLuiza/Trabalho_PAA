#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include "graph.h"
#include <iosfwd>
#include <utility>
#include <string>
#include <vector>

class DirectedGraph : public Graph {
public:
    explicit DirectedGraph(int numVertices = 0);

    static DirectedGraph fromStream(std::istream& input);
    static DirectedGraph fromFile(const std::string& filePath);
    bool isReachable(int source, int target) const;
    bool isReachableIgnoringEdge(int source, int target, int ignoreU, int ignoreV) const;
    bool isEdgeRedundant(int u, int v) const;
    std::vector<std::pair<int, int>> getEdgeListSnapshot() const;
    int removeRedundantEdgesUsingSnapshot();

    bool addEdge(int from, int to) override;
    bool removeEdge(int from, int to) override;
    bool hasEdge(int from, int to) const override;
};

#endif
