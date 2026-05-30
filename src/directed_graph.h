#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include "graph.h"

class DirectedGraph : public Graph {
public:
    explicit DirectedGraph(int numVertices = 0);

    bool addEdge(int from, int to) override;
    bool removeEdge(int from, int to) override;
    bool hasEdge(int from, int to) const override;
};

#endif
