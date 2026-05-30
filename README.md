# Trabalho_PAA

Este projeto trabalha com **grafos direcionados**.

## Formato de entrada

A entrada deve seguir exatamente este formato:

```text
n m
u1 v1
u2 v2
u3 v3
...
um vm
```

Onde:

- `n` = numero de vertices (indices de `0` ate `n-1`)
- `m` = numero de arestas
- cada linha `ui vi` representa uma aresta direcionada `ui -> vi`

## Leitura por arquivo (recomendado)

O ideal e ler o grafo de um arquivo texto.

Exemplo:

```cpp
#include "src/directed_graph.h"

DirectedGraph g = DirectedGraph::fromFile("dados/pequenos/grafo1.txt");
```

Tambem e possivel ler de qualquer `std::istream`:

```cpp
DirectedGraph g = DirectedGraph::fromStream(std::cin);
```

## Arquivos de exemplo

Foram adicionados exemplos em:

- `dados/pequenos/grafo1.txt`
- `dados/medios/grafo1.txt`
- `dados/grandes/grafo1.txt`

## Executando teste rapido

### Linux/macOS (g++)

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app src/main.cpp src/graph.cpp src/directed_graph.cpp
```

Execute (exemplo com arquivo pequeno):

```bash
./grafo_app dados/pequenos/grafo1.txt
```

### Windows PowerShell (g++)

Compile:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
```

Execute (exemplo com arquivo pequeno):

```powershell
.\grafo_app.exe dados\pequenos\grafo1.txt
```

### Testando os 3 arquivos de exemplo

Pequeno:

```powershell
.\grafo_app.exe dados\pequenos\grafo1.txt
```

Medio:

```powershell
.\grafo_app.exe dados\medios\grafo1.txt
```

Grande:

```powershell
.\grafo_app.exe dados\grandes\grafo1.txt
```

Saida esperada: o programa deve mostrar sucesso na leitura e imprimir `Vertices (n)` e `Arestas (m)` conforme a primeira linha do arquivo.

## Evolucao: DFS para atingibilidade

Foi adicionada a funcao `isReachable(source, target)` no `DirectedGraph`, usando DFS, para responder:

`Existe caminho de source ate target?` -> `true` ou `false`.

### Exemplo de compilacao e consulta de atingibilidade (Git Bash)

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo1.txt 0 4
```

A ultima linha da saida deve ser:

```text
Existe caminho de 0 ate 4? true
```

## Evolucao: DFS ignorando uma aresta

Foi adicionada a funcao `isReachableIgnoringEdge(source, target, ignoreU, ignoreV)`, que executa DFS a partir de `source`, mas ignora a aresta `ignoreU -> ignoreV`.

Regra aplicada durante a DFS:

```text
se u == ignoreU e v == ignoreV:
    pula essa aresta
senao:
    continua normalmente
```

Tambem foi adicionada a funcao `isEdgeRedundant(u, v)`, que responde se a aresta `u->v` e redundante:

- remove logicamente `u->v` (via DFS ignorando a aresta)
- verifica se ainda existe caminho de `u` ate `v`
- retorna `true` se for redundante, `false` caso contrario

### Novos modos do executavel

```text
./grafo_app.exe <arquivo_do_grafo>
./grafo_app.exe <arquivo_do_grafo> <source> <target>
./grafo_app.exe <arquivo_do_grafo> <source> <target> <ignoreU> <ignoreV>
```

### Exemplo de teste (Git Bash)

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo1.txt 1 4 1 4
```

Saida esperada no final:

```text
Existe caminho de 1 ate 4 ignorando 1->4? true
Aresta 1->4 redundante? true
```
