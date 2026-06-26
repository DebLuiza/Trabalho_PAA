import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 1. Carregamento e Processamento dos Dados
try:
    df = pd.read_csv('tempos_execucao.csv')
except FileNotFoundError:
    print("Erro: Arquivo 'tempos_execucao.csv' nao encontrado. Rode o script bash primeiro.")
    exit(1)

# Calcula a media e o desvio padrao
stats = df.groupby('Algoritmo')['Tempo_ms'].agg(['mean', 'std']).reset_index()

# Mapeamento de arestas finais documentado no projeto
arestas_finais = {
    '--reduce': 339,
    '--reduce-optimized-conservative': 909,
    '--reduce-internal-search': 339,
    '--reduce-optimized-rings': 309,
    '--reduce-dag': 299
}
stats['Arestas_Finais'] = stats['Algoritmo'].map(arestas_finais)

# Configuracao estetica geral (Estilo Academico/Profissional)
plt.rcParams.update({'font.size': 10, 'axes.axisbelow': True})
cores_scc = ['#34495e', '#2980b9', '#27ae60', '#d35400']
cor_dag = '#7f8c8d'

# ==============================================================================
# GRAFICO 1: Tempo de Execucao (Instancia Geral: grafo_scc_grande.txt)
# ==============================================================================
plt.figure(figsize=(9, 5.5))

stats_scc = stats[stats['Algoritmo'] != '--reduce-dag'].sort_values(by='mean')

bars = plt.bar(stats_scc['Algoritmo'], stats_scc['mean'], 
               yerr=stats_scc['std'], capsize=6, 
               color=cores_scc[:len(stats_scc)], edgecolor='black', alpha=0.9)

plt.title('Tempo Medio de Execucao\n(Instancia: grafo_scc_grande.txt | 10 Execucoes)', fontsize=11, fontweight='bold')
plt.ylabel('Tempo de Processamento (ms)', fontweight='bold')
plt.xlabel('Algoritmo / Parametro', fontweight='bold')
plt.xticks(rotation=15)
plt.grid(axis='y', linestyle='--', alpha=0.5)

# Correcao da sobreposicao: posiciona o texto acima do limite superior do desvio padrao
for bar, std in zip(bars, stats_scc['std']):
    height = bar.get_height()
    valor_maximo_barra = height + std
    plt.text(bar.get_x() + bar.get_width()/2., valor_maximo_barra + (stats_scc['mean'].max() * 0.03),
             f'{height:.3f} ms', ha='center', va='bottom', fontsize=9, fontweight='bold')

# Define o limite do eixo Y com uma folga para as anotacoes nao cortarem no topo
plt.ylim(0, (stats_scc['mean'].max() + stats_scc['std'].max()) * 1.25)

plt.tight_layout()
plt.savefig('grafico1_tempo.png', dpi=300)
plt.close()
print("Grafico 1 gerado: grafico1_tempo.png")


# ==============================================================================
# GRAFICO 2: Comparacao de Arestas Finais (Compressao)
# ==============================================================================
plt.figure(figsize=(9, 5.5))

stats_arestas = stats.sort_values(by='Arestas_Finais', ascending=False)
cores_barras_2 = [cor_dag if algo == '--reduce-dag' else '#2c3e50' for algo in stats_arestas['Algoritmo']]

bars2 = plt.bar(stats_arestas['Algoritmo'], stats_arestas['Arestas_Finais'], 
                color=cores_barras_2, edgecolor='black', alpha=0.9)

plt.title('Eficiencia de Reducao: Quantidade de Arestas Finais\n(Arestas Iniciais: SCC = 924 | DAG = 1137)', fontsize=11, fontweight='bold')
plt.ylabel('Numero de Arestas Restantes', fontweight='bold')
plt.xlabel('Algoritmo / Parametro', fontweight='bold')
plt.xticks(rotation=15)
plt.grid(axis='y', linestyle='--', alpha=0.5)

for bar in bars2:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height + (stats_arestas['Arestas_Finais'].max() * 0.02),
             f'{int(height)}', ha='center', va='bottom', fontsize=9, fontweight='bold')

plt.ylim(0, stats_arestas['Arestas_Finais'].max() * 1.15)

# Correcao da sobreposicao da nota: define uma margem explicita inferior antes de plotar o texto fixo
plt.tight_layout(rect=[0, 0.08, 1, 1])
plt.figtext(0.5, 0.02, "*Nota: O modo '--reduce-dag' foi avaliado sobre a instancia aciclica 'grafo_dag_grande.txt'.", 
            fontsize=8.5, style='italic', color='#555555', ha='center')

plt.savefig('grafico2_arestas.png', dpi=300)
plt.close()
print("Grafico 2 gerado: grafico2_arestas.png")


# ==============================================================================
# GRAFICO 3: Trade-off Qualidade vs. Velocidade
# ==============================================================================
plt.figure(figsize=(9, 5.5))

stats_tradeoff = stats[stats['Algoritmo'] != '--reduce-dag']

plt.scatter(stats_tradeoff['Arestas_Finais'], stats_tradeoff['mean'], 
            s=120, color='#8e44ad', edgecolor='black', zorder=3)

# Correcao da sobreposicao das caixas: deslocamento otimizado para nao colidir com as linhas de grade
for idx, row in stats_tradeoff.iterrows():
    plt.annotate(row['Algoritmo'], 
                 (row['Arestas_Finais'], row['mean']),
                 textcoords="offset points", 
                 xytext=(0, 12), 
                 ha='center', 
                 fontsize=8.5, 
                 fontweight='bold',
                 bbox=dict(boxstyle="round,pad=0.2", fc="#ffffff", ec="#b2bec3", alpha=0.9))

plt.title('Analise de Trade-off: Qualidade da Reducao vs. Velocidade\n(Proximidade com a origem [0,0] indica melhor custo-beneficio)', fontsize=11, fontweight='bold')
plt.xlabel('Numero de Arestas Finais (Menor e Melhor)', fontweight='bold')
plt.ylabel('Tempo Medio de Execucao (ms) (Menor e Melhor)', fontweight='bold')
plt.grid(True, linestyle='--', alpha=0.5)

plt.xlim(stats_tradeoff['Arestas_Finais'].min() - 60, stats_tradeoff['Arestas_Finais'].max() + 60)
plt.ylim(stats_tradeoff['mean'].min() - 0.15, stats_tradeoff['mean'].max() + 0.15)

plt.tight_layout()
plt.savefig('grafico3_tradeoff.png', dpi=300)
plt.close()
print("Grafico 3 gerado: grafico3_tradeoff.png")
