#include "directed_graph.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3 && argc != 4 && argc != 6) {
        std::cerr << "Uso: " << argv[0]
                  << " <arquivo_do_grafo> [--scc|--condensation|--reduce-dag|--reduce-condensation|--reduce-optimized-conservative|--reduce-internal-search|--reduce-optimized-rings|--reduce] [source target] [source target ignoreU ignoreV]\n";
        return 1;
    }

    try {
        const std::string filePath = argv[1];
        DirectedGraph graph = DirectedGraph::fromFile(filePath);

        std::cout << "Arquivo lido com sucesso: " << filePath << '\n';
        std::cout << "Vertices (n): " << graph.vertexCount() << '\n';
        std::cout << "Arestas (m): " << graph.edgeCount() << '\n';
        graph.printAdjacencyList(std::cout);

        if (argc == 3) {
            const std::string option = argv[2];
            
            if (option == "--scc") {
                const auto components = graph.stronglyConnectedComponents();
                std::cout << "Componentes fortemente conexas: " << components.size() << '\n';
                for (std::size_t i = 0; i < components.size(); ++i) {
                    std::cout << "C" << i << ":";
                    for (int vertex : components[i]) {
                        std::cout << " " << vertex;
                    }
                    std::cout << '\n';
                }
            } 
            else if (option == "--condensation") {
                const auto components = graph.stronglyConnectedComponents();
                const DirectedGraph condensation = graph.buildCondensationGraph(components);

                std::cout << "Componentes fortemente conexas: " << components.size() << '\n';
                for (std::size_t i = 0; i < components.size(); ++i) {
                    std::cout << "C" << i << ":";
                    for (int vertex : components[i]) {
                        std::cout << " " << vertex;
                    }
                    std::cout << '\n';
                }

                std::cout << "Grafo de condensacao:\n";
                condensation.printAdjacencyList(std::cout);
            } 
            else if (option == "--reduce-dag") {
                if (!graph.isDAG()) {
                    std::cerr << "Erro: --reduce-dag exige que o grafo de entrada seja um DAG.\n";
                    std::cerr << "Para grafos gerais, use --reduce, --reduce-optimized-conservative, "
                              << "--reduce-internal-search ou --reduce-optimized-rings.\n";
                    return 1;
                }

                const DirectedGraph reduced = graph.transitiveReductionDAG();
                std::cout << "Modo: reducao transitiva assumindo que a entrada ja e um DAG.\n";
                std::cout << "Reducao transitiva do DAG:\n";
                reduced.printAdjacencyList(std::cout);
            } 
            else if (option == "--reduce-condensation") {
                const auto components = graph.stronglyConnectedComponents();
                const DirectedGraph condensation = graph.buildCondensationGraph(components);
                const DirectedGraph reducedCondensation = condensation.transitiveReductionDAG();

                std::cout << "Grafo de condensacao:\n";
                condensation.printAdjacencyList(std::cout);
                std::cout << "Reducao transitiva do DAG de condensacao:\n";
                reducedCondensation.printAdjacencyList(std::cout);
            } 
            else if (option == "--reduce-optimized-conservative" || option == "--reduce-optimized") {
                const DirectedGraph reduced = graph.optimizedReductionByCondensation();
                std::cout << "Grafo reduzido por condensacao (Conservador - Mantem SCCs intactas):\n";
                reduced.printAdjacencyList(std::cout);
            } 
            else if (option == "--reduce-internal-search") {
                const DirectedGraph reduced = graph.optimizedReductionWithInternalSearch();
                std::cout << "Grafo reduzido com busca interna (Heuristica MEG para subgrafos):\n";
                reduced.printAdjacencyList(std::cout);
            }
            else if (option == "--reduce-optimized-rings") {
                const DirectedGraph reduced = graph.optimizedReductionWithSccRings();
                std::cout << "Grafo reduzido por condensacao com construcao de aneis nas SCCs:\n";
                reduced.printAdjacencyList(std::cout);
            } 
            else if (option == "--reduce") {
                const int removed = graph.removeRedundantEdgesUsingSnapshot();
                std::cout << "Modo: baseline geral por DFS ignorando cada aresta do grafo original.\n";
                std::cout << "Arestas redundantes removidas: " << removed << '\n';
                std::cout << "Grafo apos reducao:\n";
                graph.printAdjacencyList(std::cout);
            } 
            else {
                std::cerr << "Opcao invalida: " << option << '\n';
                return 1;
            }
        } 
        else if (argc == 4) {
            const int source = std::stoi(argv[2]);
            const int target = std::stoi(argv[3]);
            const bool reachable = graph.isReachable(source, target);
            std::cout << "Existe caminho de " << source << " ate " << target << "? "
                      << (reachable ? "true" : "false") << '\n';
        } 
        else if (argc == 6) {
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
        std::cerr << "Erro ao ler grafo ou processar comando: " << ex.what() << '\n';
        return 1;
    }
}