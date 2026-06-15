# Comparacao dos resultados

## Ambiente dos testes

Os resultados abaixo foram obtidos a partir dos arquivos gerados pelo programa em uma execucao de cada algoritmo. O tempo exibido corresponde a linha `Tempo de processamento` impressa pelo executavel.

Como os tempos podem variar entre execucoes, eles devem ser interpretados como uma comparacao preliminar. Para o relatorio final, o ideal e executar cada caso algumas vezes e usar a media.

## Criterio de comparacao

Para manter a comparacao justa, todas as implementacoes aplicaveis a grafos gerais foram avaliadas sobre a mesma instancia:

`dados/grandes/grafo_scc_grande.txt`

A unica excecao e `--reduce-dag`, pois esse modo exige que o grafo de entrada seja aciclico. Por isso, ele foi testado separadamente em:

`dados/grandes/grafo_dag_grande.txt`

## Instancias usadas

| Arquivo | Descricao | Vertices | Arestas iniciais |
|---|---:|---:|---:|
| `dados/grandes/grafo_scc_grande.txt` | Grafo com SCCs grandes e redundancias internas | 300 | 924 |
| `dados/grandes/grafo_dag_grande.txt` | DAG usado apenas para validar `--reduce-dag` | 300 | 1137 |

## Resultados

| Comando | Instancia | Tempo | Arestas finais | Observacao |
|---|---|---:|---:|---|
| `--reduce` | `grafo_scc_grande.txt` | 0.637 ms | 339 | Baseline geral por DFS ignorando cada aresta. |
| `--reduce-optimized-conservative` | `grafo_scc_grande.txt` | 0.077 ms | 909 | Removeu apenas redundancias entre SCCs; manteve SCCs internas intactas. |
| `--reduce-internal-search` | `grafo_scc_grande.txt` | 0.329 ms | 339 | Resultado equivalente ao baseline, mas usando SCCs e busca interna restrita. |
| `--reduce-optimized-rings` | `grafo_scc_grande.txt` | 0.069 ms | 309 | Menor numero de arestas, mas pode criar arestas artificiais dentro das SCCs. |
| `--reduce-dag` | `grafo_dag_grande.txt` | 0.333 ms | 299 | Teste separado, pois o modo exige entrada aciclica. |

## Leitura dos resultados

O algoritmo `--reduce-dag` se comportou como esperado para a instancia aciclica: removeu as arestas transitivas e deixou uma estrutura essencialmente linear com 299 arestas.

No grafo com SCCs, o baseline `--reduce` e o algoritmo `--reduce-internal-search` chegaram ao mesmo numero de arestas finais: 339. Esse e um resultado importante, pois indica que a busca interna nas SCCs preservou o comportamento da reducao por aresta, mas com uma estrutura mais alinhada a decomposicao por componentes fortemente conexas.

O modo `--reduce-optimized-conservative` foi o mais rapido entre as versoes de subgrafo, mas reduziu pouco: de 924 para 909 arestas. Isso acontece porque ele preserva todas as arestas internas das SCCs e so remove redundancias no DAG de condensacao.

O modo `--reduce-optimized-rings` gerou o menor grafo, com 309 arestas. Entretanto, essa versao substitui a estrutura interna das SCCs por aneis, podendo adicionar arestas que nao existiam originalmente. Assim, ela e adequada para discutir equivalencia de atingibilidade, mas nao deve ser apresentada como reducao por subgrafo estrito.

## Conclusao breve

Para a entrega principal, o algoritmo mais equilibrado e `--reduce-internal-search`: ele usa Tarjan, condensacao em DAG e busca interna nas SCCs, mantendo apenas arestas originais. Nos testes grandes, ele produziu o mesmo numero de arestas do baseline geral (`--reduce`), mas com uma formulacao mais estruturada e teoricamente alinhada ao tratamento de grafos gerais.

O baseline `--reduce` deve ser mantido como referencia de corretude. A versao conservadora e util para mostrar a etapa intermediaria da condensacao. A versao com aneis deve aparecer como alternativa teorica baseada em preservacao de atingibilidade, com a ressalva de que nao necessariamente produz subgrafo do grafo original.
