#include "directed_graph.h"

#include <algorithm>
#include <fstream>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <vector>

DirectedGraph::DirectedGraph(int numVertices)
    : Graph(numVertices) {}

DirectedGraph DirectedGraph::fromStream(std::istream& input) {
    // Accept optional UTF-8 BOM at file start.
    if (input.peek() == 0xEF) {
        const int b1 = input.get();
        const int b2 = input.get();
        const int b3 = input.get();
        if (!(b1 == 0xEF && b2 == 0xBB && b3 == 0xBF)) {
            throw std::invalid_argument("Invalid file header.");
        }
    }

    int n = 0;
    int m = 0;
    if (!(input >> n >> m)) {
        throw std::invalid_argument("Failed to read n and m.");
    }

    if (n < 0 || m < 0) {
        throw std::invalid_argument("n and m must be non-negative.");
    }

    DirectedGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int from = 0;
        int to = 0;
        if (!(input >> from >> to)) {
            throw std::invalid_argument("Failed to read edge list.");
        }
        if (!graph.addEdge(from, to)) {
            std::ostringstream message;
            message << "Invalid or duplicate edge: " << from << " -> " << to << ".";
            throw std::invalid_argument(message.str());
        }
    }

    return graph;
}

DirectedGraph DirectedGraph::fromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    return fromStream(file);
}

bool DirectedGraph::isReachable(int source, int target) const {
    if (!isValidVertex(source) || !isValidVertex(target)) {
        return false;
    }

    if (source == target) {
        return true;
    }

    std::vector<bool> visited(static_cast<size_t>(vertexCount()), false);
    std::stack<int> toVisit;
    toVisit.push(source);
    visited[static_cast<size_t>(source)] = true;

    while (!toVisit.empty()) {
        const int current = toVisit.top();
        toVisit.pop();

        const std::vector<int>& neighbors = adjacentTo(current);
        for (int neighbor : neighbors) {
            if (neighbor == target) {
                return true;
            }
            if (!visited[static_cast<size_t>(neighbor)]) {
                visited[static_cast<size_t>(neighbor)] = true;
                toVisit.push(neighbor);
            }
        }
    }

    return false;
}

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
