#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include "graph.h"
#include <iosfwd>
#include <string>

class DirectedGraph : public Graph {
public:
    explicit DirectedGraph(int numVertices = 0);

    static DirectedGraph fromStream(std::istream& input);
    static DirectedGraph fromFile(const std::string& filePath);

    bool addEdge(int from, int to) override;
    bool removeEdge(int from, int to) override;
    bool hasEdge(int from, int to) const override;
};

#endif
