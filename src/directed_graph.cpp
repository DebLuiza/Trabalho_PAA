#include "directed_graph.h"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
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

// ---------------------------------------------------------------------------
// Algoritmo 1: reducao simples por DFS
// ---------------------------------------------------------------------------

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

bool DirectedGraph::isReachableIgnoringEdge(int source, int target, int ignoreU, int ignoreV) const {
    if (!isValidVertex(source) || !isValidVertex(target)) {
        return false;
    }
    if (!isValidVertex(ignoreU) || !isValidVertex(ignoreV)) {
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
            if (current == ignoreU && neighbor == ignoreV) {
                continue;
            }

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

bool DirectedGraph::isEdgeRedundant(int u, int v) const {
    if (!hasEdge(u, v)) {
        return false;
    }
    return isReachableIgnoringEdge(u, v, u, v);
}

std::vector<std::pair<int, int>> DirectedGraph::getEdgeListSnapshot() const {
    std::vector<std::pair<int, int>> edges;
    edges.reserve(static_cast<size_t>(edgeCount()));

    for (int u = 0; u < vertexCount(); ++u) {
        const std::vector<int>& neighbors = adjacentTo(u);
        for (int v : neighbors) {
            edges.emplace_back(u, v);
        }
    }

    return edges;
}

int DirectedGraph::removeRedundantEdgesUsingSnapshot() {
    const std::vector<std::pair<int, int>> originalEdges = getEdgeListSnapshot();
    int removedCount = 0;

    for (const std::pair<int, int>& edge : originalEdges) {
        const int u = edge.first;
        const int v = edge.second;

        // The graph may have changed after prior removals. Skip if already removed.
        if (!hasEdge(u, v)) {
            continue;
        }

        if (isReachableIgnoringEdge(u, v, u, v)) {
            if (removeEdge(u, v)) {
                ++removedCount;
            }
        }
    }

    return removedCount;
}

// ---------------------------------------------------------------------------
// Algoritmo 2: base otimizada por SCCs (Tarjan) e DAG de condensacao
// ---------------------------------------------------------------------------

std::vector<std::vector<int>> DirectedGraph::stronglyConnectedComponents() const {
    const int n = vertexCount();
    int nextIndex = 0;
    std::vector<int> index(static_cast<size_t>(n), -1);
    std::vector<int> lowlink(static_cast<size_t>(n), 0);
    std::vector<bool> onStack(static_cast<size_t>(n), false);
    std::stack<int> stack;
    std::vector<std::vector<int>> components;

    std::function<void(int)> dfs = [&](int u) {
        index[static_cast<size_t>(u)] = nextIndex;
        lowlink[static_cast<size_t>(u)] = nextIndex;
        ++nextIndex;
        stack.push(u);
        onStack[static_cast<size_t>(u)] = true;

        const std::vector<int>& neighbors = adjacentTo(u);
        for (int v : neighbors) {
            if (index[static_cast<size_t>(v)] == -1) {
                dfs(v);
                lowlink[static_cast<size_t>(u)] =
                    std::min(lowlink[static_cast<size_t>(u)], lowlink[static_cast<size_t>(v)]);
            } else if (onStack[static_cast<size_t>(v)]) {
                lowlink[static_cast<size_t>(u)] =
                    std::min(lowlink[static_cast<size_t>(u)], index[static_cast<size_t>(v)]);
            }
        }

        if (lowlink[static_cast<size_t>(u)] == index[static_cast<size_t>(u)]) {
            std::vector<int> component;
            int v = -1;
            do {
                v = stack.top();
                stack.pop();
                onStack[static_cast<size_t>(v)] = false;
                component.push_back(v);
            } while (v != u);

            std::sort(component.begin(), component.end());
            components.push_back(component);
        }
    };

    for (int vertex = 0; vertex < n; ++vertex) {
        if (index[static_cast<size_t>(vertex)] == -1) {
            dfs(vertex);
        }
    }

    return components;
}

std::vector<int> DirectedGraph::componentIndexByVertex(
    const std::vector<std::vector<int>>& components
) const {
    std::vector<int> componentOfVertex(static_cast<size_t>(vertexCount()), -1);

    for (std::size_t componentIndex = 0; componentIndex < components.size(); ++componentIndex) {
        for (int vertex : components[componentIndex]) {
            if (!isValidVertex(vertex)) {
                throw std::invalid_argument("Component contains invalid vertex.");
            }
            componentOfVertex[static_cast<size_t>(vertex)] =
                static_cast<int>(componentIndex);
        }
    }

    return componentOfVertex;
}

DirectedGraph DirectedGraph::buildCondensationGraph(
    const std::vector<std::vector<int>>& components
) const {
    const std::vector<int> componentOfVertex = componentIndexByVertex(components);
    DirectedGraph condensation(static_cast<int>(components.size()));

    for (int u = 0; u < vertexCount(); ++u) {
        const int componentU = componentOfVertex[static_cast<size_t>(u)];
        if (componentU == -1) {
            throw std::invalid_argument("Missing component for vertex.");
        }

        const std::vector<int>& neighbors = adjacentTo(u);
        for (int v : neighbors) {
            const int componentV = componentOfVertex[static_cast<size_t>(v)];
            if (componentV == -1) {
                throw std::invalid_argument("Missing component for vertex.");
            }
            if (componentU != componentV) {
                condensation.addEdge(componentU, componentV);
            }
        }
    }

    return condensation;
}

DirectedGraph DirectedGraph::transitiveReductionDAG() const {
    DirectedGraph reduced = *this;
    const std::vector<std::pair<int, int>> originalEdges = getEdgeListSnapshot();

    for (const std::pair<int, int>& edge : originalEdges) {
        const int u = edge.first;
        const int v = edge.second;

        if (!reduced.hasEdge(u, v)) {
            continue;
        }

        if (reduced.isReachableIgnoringEdge(u, v, u, v)) {
            reduced.removeEdge(u, v);
        }
    }

    return reduced;
}

DirectedGraph DirectedGraph::optimizedReductionByCondensation() const {
    const std::vector<std::vector<int>> components = stronglyConnectedComponents();
    const std::vector<int> componentOfVertex = componentIndexByVertex(components);
    const DirectedGraph condensation = buildCondensationGraph(components);
    const DirectedGraph reducedCondensation = condensation.transitiveReductionDAG();

    DirectedGraph reduced = *this;
    const std::vector<std::pair<int, int>> originalEdges = getEdgeListSnapshot();

    for (const std::pair<int, int>& edge : originalEdges) {
        const int u = edge.first;
        const int v = edge.second;
        const int componentU = componentOfVertex[static_cast<size_t>(u)];
        const int componentV = componentOfVertex[static_cast<size_t>(v)];

        if (componentU == componentV) {
            continue;
        }

        if (!reducedCondensation.hasEdge(componentU, componentV)) {
            reduced.removeEdge(u, v);
        }
    }

    return reduced;
}

DirectedGraph DirectedGraph::optimizedReductionWithSccRings() const {
    const std::vector<std::vector<int>> components = stronglyConnectedComponents();
    const std::vector<int> componentOfVertex = componentIndexByVertex(components);
    const DirectedGraph condensation = buildCondensationGraph(components);
    const DirectedGraph reducedCondensation = condensation.transitiveReductionDAG();

    DirectedGraph reduced(vertexCount());

    for (const std::vector<int>& component : components) {
        if (component.size() <= 1) {
            continue;
        }

        for (std::size_t i = 0; i < component.size(); ++i) {
            const int from = component[i];
            const int to = component[(i + 1) % component.size()];
            reduced.addEdge(from, to);
        }
    }

    const std::vector<std::pair<int, int>> originalEdges = getEdgeListSnapshot();
    for (const std::pair<int, int>& edge : originalEdges) {
        const int u = edge.first;
        const int v = edge.second;
        const int componentU = componentOfVertex[static_cast<size_t>(u)];
        const int componentV = componentOfVertex[static_cast<size_t>(v)];

        if (componentU == componentV) {
            continue;
        }

        if (reducedCondensation.hasEdge(componentU, componentV)) {
            reduced.addEdge(u, v);
        }
    }

    return reduced;
}

// ---------------------------------------------------------------------------
// Operacoes basicas de grafo direcionado
// ---------------------------------------------------------------------------

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
