#include "directed_graph.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4 && argc != 6) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_do_grafo> [source target] [source target ignoreU ignoreV]\n";
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
        } else if (argc == 6) {
            const int source = std::stoi(argv[2]);
            const int target = std::stoi(argv[3]);
            const int ignoreU = std::stoi(argv[4]);
            const int ignoreV = std::stoi(argv[5]);

            const bool reachableIgnoringEdge =
                graph.isReachableIgnoringEdge(source, target, ignoreU, ignoreV);
            std::cout << "Existe caminho de " << source << " ate " << target
                      << " ignorando " << ignoreU << "->" << ignoreV << "? "
                      << (reachableIgnoringEdge ? "true" : "false") << '\n';

            if (source == ignoreU && target == ignoreV) {
                std::cout << "Aresta " << ignoreU << "->" << ignoreV << " redundante? "
                          << (graph.isEdgeRedundant(ignoreU, ignoreV) ? "true" : "false")
                          << '\n';
            }
        }
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Erro ao ler grafo: " << ex.what() << '\n';
        return 1;
    }
}
