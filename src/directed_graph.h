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

    // Algoritmo 1: reducao simples por DFS.
    bool isReachable(int source, int target) const;
    bool isReachableIgnoringEdge(int source, int target, int ignoreU, int ignoreV) const;
    bool isEdgeRedundant(int u, int v) const;
    std::vector<std::pair<int, int>> getEdgeListSnapshot() const;
    int removeRedundantEdgesUsingSnapshot();

    // Algoritmo 2: base otimizada por SCCs (Tarjan) e DAG de condensacao.
    std::vector<std::vector<int>> stronglyConnectedComponents() const;
    std::vector<int> componentIndexByVertex(
        const std::vector<std::vector<int>>& components
    ) const;
    DirectedGraph buildCondensationGraph(
        const std::vector<std::vector<int>>& components
    ) const;
    bool isDAG() const;
    DirectedGraph transitiveReductionDAG() const;
    DirectedGraph optimizedReductionByCondensation() const;
    DirectedGraph optimizedReductionWithSccRings() const;

    bool addEdge(int from, int to) override;
    bool removeEdge(int from, int to) override;
    bool hasEdge(int from, int to) const override;
};

#endif
