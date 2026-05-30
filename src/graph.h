#ifndef GRAPH_H
#define GRAPH_H

#include <iosfwd>
#include <vector>

class Graph {
public:
    explicit Graph(int numVertices = 0);
    virtual ~Graph() = default;

    int vertexCount() const;
    int edgeCount() const;

    void addVertex();
    virtual bool addEdge(int from, int to) = 0;
    virtual bool removeEdge(int from, int to) = 0;
    virtual bool hasEdge(int from, int to) const = 0;

    const std::vector<int>& adjacentTo(int vertex) const;
    void printAdjacencyList(std::ostream& output) const;

protected:
    bool isValidVertex(int vertex) const;

    std::vector<std::vector<int>> adjacencyList_;
    int edgeCount_;
};

#endif
