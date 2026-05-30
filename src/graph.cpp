#include "graph.h"

#include <ostream>
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

void Graph::printAdjacencyList(std::ostream& output) const {
    for (int vertex = 0; vertex < vertexCount(); ++vertex) {
        output << vertex << ":";
        const std::vector<int>& neighbors = adjacencyList_[vertex];
        for (int neighbor : neighbors) {
            output << " " << neighbor;
        }
        output << '\n';
    }
}

bool Graph::isValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertexCount();
}
