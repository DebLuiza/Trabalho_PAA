#!/bin/bash

# ==============================================================================
# Script de Coleta Automatizada de Dados de Tempo - PAA
# ==============================================================================

# 1. Compilação Limpa do Projeto
echo "=================================================="
echo "[1/3] Compilando o projeto com otimização -O2..."
echo "=================================================="

# Compilação padrão Linux/macOS
g++ -std=c++17 -O2 -Wall -Wextra -o grafo_app src/main.cpp src/graph.cpp src/directed_graph.cpp

# Valida se a compilação funcionou antes de prosseguir
if [ $? -ne 0 ]; then
    echo "Erro crítico na compilação! Verifique o código-fonte antes de rodar o script."
    exit 1
fi

echo "Compilação concluída com sucesso! Executável 'grafo_app' gerado."
echo ""

# 2. Configuração do Arquivo de Saída CSV
OUTPUT_CSV="tempos_execucao.csv"
echo "Algoritmo,Instancia,Execucao,Tempo_ms" > "$OUTPUT_CSV"

# Definição dos modos e seus respectivos arquivos de teste (alinhado com o repositório)
MODOS=(
    "--reduce"
    "--reduce-optimized-conservative"
    "--reduce-internal-search"
    "--reduce-optimized-rings"
    "--reduce-dag"
)

ARQUIVOS=(
    "dados/grandes/grafo_scc_grande.txt"
    "dados/grandes/grafo_scc_grande.txt"
    "dados/grandes/grafo_scc_grande.txt"
    "dados/grandes/grafo_scc_grande.txt"
    "dados/grandes/grafo_dag_grande.txt"
)

N_EXECUCOES=10

echo "=================================================="
echo "[2/3] Iniciando bateria de testes (10x por modo)"
echo "=================================================="

# Loop principal pelas configurações de algoritmos
for i in "${!MODOS[@]}"; do
    modo="${MODOS[$i]}"
    arquivo="${ARQUIVOS[$i]}"
    
    echo "   Processando algoritmo: $modo"
    echo "   Arquivo alvo: $arquivo"
    
    for run in $(seq 1 $N_EXECUCOES); do
        # Executa o programa e captura o output completo
        saida=$(./grafo_app "$arquivo" "$modo" 2>&1)
        
        # Filtra a linha correspondente ao tempo e extrai o valor numérico
        # Padrão esperado: "Tempo de processamento: X.XXX ms"
        tempo=$(echo "$saida" | grep "Tempo de processamento" | awk '{print $4}')
        
        # Tratamento simples caso o executável falhe ou mude o formato da string
        if [ -z "$tempo" ]; then
            echo "      [Erro] Falha ao coletar dados do tempo na execução $run"
            continue
        fi
        
        # Salva o resultado linha por linha no formato CSV
        echo "$modo,$arquivo,$run,$tempo" >> "$OUTPUT_CSV"
        echo "   └─ Execução #$run: $tempo ms"
    done
    echo "--------------------------------------------------"
done

echo ""
echo "=================================================="
echo "[3/3] Coleta finalizada com sucesso!"
echo "=================================================="
