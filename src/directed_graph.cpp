#include "directed_graph.h"

#include <algorithm>

DirectedGraph::DirectedGraph(int numVertices)
    : Graph(numVertices) {}

bool DirectedGraph::addEdge(int from, int to) {
    if (!isValidVertex(from) || !isValidVertex(to)) {
        return false;
    }

    std::vector<int>& neighbors = adjacencyList_[from];
    if (std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end()) {
        return false;
    }

    neighbors.push_back(to);
    ++edgeCount_;
    return true;
}

bool DirectedGraph::removeEdge(int from, int to) {
    if (!isValidVertex(from) || !isValidVertex(to)) {
        return false;
    }

    std::vector<int>& neighbors = adjacencyList_[from];
    std::vector<int>::iterator it = std::find(neighbors.begin(), neighbors.end(), to);
    if (it == neighbors.end()) {
        return false;
    }

    neighbors.erase(it);
    --edgeCount_;
    return true;
}

bool DirectedGraph::hasEdge(int from, int to) const {
    if (!isValidVertex(from) || !isValidVertex(to)) {
        return false;
    }

    const std::vector<int>& neighbors = adjacencyList_[from];
    return std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
}
