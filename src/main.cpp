#include "directed_graph.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_do_grafo>\n";
        return 1;
    }

    try {
        const std::string filePath = argv[1];
        DirectedGraph graph = DirectedGraph::fromFile(filePath);

        std::cout << "Arquivo lido com sucesso: " << filePath << '\n';
        std::cout << "Vertices (n): " << graph.vertexCount() << '\n';
        std::cout << "Arestas (m): " << graph.edgeCount() << '\n';
        graph.printAdjacencyList(std::cout);
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Erro ao ler grafo: " << ex.what() << '\n';
        return 1;
    }
}
