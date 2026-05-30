#include "directed_graph.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_do_grafo> [source target]\n";
        return 1;
    }

    try {
        const std::string filePath = argv[1];
        DirectedGraph graph = DirectedGraph::fromFile(filePath);

        std::cout << "Arquivo lido com sucesso: " << filePath << '\n';
        std::cout << "Vertices (n): " << graph.vertexCount() << '\n';
        std::cout << "Arestas (m): " << graph.edgeCount() << '\n';
        graph.printAdjacencyList(std::cout);

        if (argc == 4) {
            const int source = std::stoi(argv[2]);
            const int target = std::stoi(argv[3]);
            const bool reachable = graph.isReachable(source, target);
            std::cout << "Existe caminho de " << source << " ate " << target << "? "
                      << (reachable ? "true" : "false") << '\n';
        }
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Erro ao ler grafo: " << ex.what() << '\n';
        return 1;
    }
}
