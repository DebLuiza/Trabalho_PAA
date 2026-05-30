#include "graph.h"

#include <stdexcept>

Graph::Graph(int numVertices)
    : adjacencyList_(numVertices), edgeCount_(0) {}

int Graph::vertexCount() const {
    return static_cast<int>(adjacencyList_.size());
}

int Graph::edgeCount() const {
    return edgeCount_;
}

void Graph::addVertex() {
    adjacencyList_.push_back({});
}

const std::vector<int>& Graph::adjacentTo(int vertex) const {
    if (!isValidVertex(vertex)) {
        throw std::out_of_range("Vertex index is out of bounds.");
    }
    return adjacencyList_[vertex];
}

bool Graph::isValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertexCount();
}
