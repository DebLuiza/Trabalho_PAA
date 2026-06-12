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
./grafo_app.exe <arquivo_do_grafo> --scc
./grafo_app.exe <arquivo_do_grafo> --condensation
./grafo_app.exe <arquivo_do_grafo> --reduce-dag
./grafo_app.exe <arquivo_do_grafo> --reduce-condensation
./grafo_app.exe <arquivo_do_grafo> --reduce-optimized-conservative
./grafo_app.exe <arquivo_do_grafo> --reduce-optimized-rings
./grafo_app.exe <arquivo_do_grafo> --reduce
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

## Organizacao das implementacoes

O projeto mantem duas linhas de implementacao para comparacao:

- Algoritmo 1: reducao simples por DFS ignorando uma aresta.
- Algoritmo 2: base otimizada com Tarjan para SCCs e futura condensacao em DAG.

Mesmo dentro de `DirectedGraph`, o codigo foi separado por blocos nomeados para deixar claro onde termina a abordagem simples e onde comeca a abordagem otimizada.

## Evolucao: reducao com snapshot de arestas

Para evitar problemas de iteracao ao remover arestas durante o processamento, a reducao usa uma copia fixa das arestas originais (snapshot).

Fluxo:

1. captura a lista original de arestas;
2. testa redundancia em cada aresta da copia;
3. remove do grafo atual somente as redundantes.

### Teste da reducao completa

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo1.txt --reduce
```

Saida esperada:

- quantidade de arestas removidas:
  `Arestas redundantes removidas: X`
- lista de adjacencia final apos reducao.

## Evolucao: Tarjan para componentes fortemente conexas

Foi adicionada a funcao `stronglyConnectedComponents()`, baseada no algoritmo de Tarjan, para encontrar SCCs do grafo direcionado.

### Teste de SCCs

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo_scc.txt --scc
```

Para esse arquivo, a estrutura esperada das componentes e:

```text
{0, 1, 2}
{3, 4}
{5}
```

A ordem de impressao das componentes pode variar, mas os grupos de vertices devem ser esses.

## Evolucao: grafo de condensacao

Foi adicionada a construcao do DAG de condensacao:

- cada SCC vira um vertice;
- arestas internas a mesma SCC sao ignoradas;
- arestas entre SCCs diferentes viram arestas no DAG;
- arestas duplicadas entre componentes sao evitadas.

### Teste da condensacao

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo_scc.txt --condensation
```

Para `grafo_scc.txt`, a estrutura esperada do DAG e:

```text
{0, 1, 2} -> {3, 4} -> {5}
```

Na numeracao impressa pelo programa, isso deve aparecer como duas arestas entre componentes consecutivas.

## Evolucao: reducao transitiva no DAG

Foi adicionada a funcao `transitiveReductionDAG()`, que aplica reducao transitiva em um DAG usando a mesma ideia de testar caminhos alternativos ignorando uma aresta.

Esse modo deve ser usado quando o grafo de entrada ja e um DAG, ou seja, quando nao ha ciclos.

Ele e diferente de `--reduce`:

- `--reduce-dag`: aplica reducao transitiva assumindo que o grafo carregado ja e um DAG.
- `--reduce`: baseline geral; testa cada aresta do grafo original com DFS ignorando essa aresta, sem assumir que o grafo e DAG.

### Teste da reducao em DAG

O arquivo `dados/pequenos/grafo_dag.txt` contem:

```text
0 -> 1
1 -> 2
0 -> 2
```

A aresta `0 -> 2` e redundante, pois existe o caminho `0 -> 1 -> 2`.

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo_dag.txt --reduce-dag
```

Saida esperada do DAG reduzido:

```text
0: 1
1: 2
2:
```

Tambem e possivel reduzir diretamente o DAG de condensacao de um grafo geral:

```bash
./grafo_app.exe dados/pequenos/grafo_scc.txt --reduce-condensation
```

### Comparacao com o baseline geral

```bash
./grafo_app.exe dados/pequenos/grafo_condensation_redundant.txt --reduce
```

Nesse caso, o programa nao assume que a entrada e DAG. Ele percorre as arestas originais e testa redundancia uma a uma por DFS ignorando a aresta testada.

## Evolucao: reducao otimizada conservadora por condensacao

Foi adicionada a funcao `optimizedReductionByCondensation()`. Ela aplica a reducao transitiva no DAG de condensacao e mapeia o resultado de volta para o grafo original.

Nesta primeira versao conservadora:

- arestas internas a uma SCC sao preservadas;
- arestas entre SCCs sao removidas quando a aresta correspondente no DAG de condensacao reduzido nao existe mais;
- se uma aresta entre SCCs permanece necessaria no DAG reduzido, as arestas originais entre essas SCCs sao mantidas.

### Teste da reducao otimizada

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo_condensation_redundant.txt --reduce-optimized-conservative
```

O modo antigo `--reduce-optimized` tambem e aceito como alias, mas o nome recomendado e `--reduce-optimized-conservative`.

Nesse exemplo, a aresta `2 -> 5` e redundante no nivel da condensacao, pois existe caminho de `2` ate `5` passando pela SCC `{3, 4}`.

No grafo reduzido, a linha do vertice `2` deve deixar de apontar diretamente para `5`:

```text
2: 0 3
```

## Evolucao: reducao otimizada com aneis nas SCCs

Foi adicionada a funcao `optimizedReductionWithSccRings()`. Ela aplica a reducao por condensacao e, dentro de cada SCC com mais de um vertice, substitui as arestas internas por um ciclo dirigido simples.

Exemplo para uma SCC `{v1, v2, v3, v4}`:

```text
v1 -> v2
v2 -> v3
v3 -> v4
v4 -> v1
```

Essa abordagem preserva a atingibilidade entre os vertices da SCC, pois todos continuam alcancando todos. Importante: essa versao pode adicionar arestas que nao existiam no grafo original, entao ela produz um grafo equivalente em atingibilidade, mas nao necessariamente um subgrafo do grafo original.

### Teste com aneis nas SCCs

```bash
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app.exe src/main.cpp src/graph.cpp src/directed_graph.cpp
./grafo_app.exe dados/pequenos/grafo_condensation_redundant.txt --reduce-optimized-rings
```
