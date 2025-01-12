import networkx as nx
import matplotlib.colors as mcolors

# Função para carregar o grafo a partir do arquivo
def carregar_grafo(arquivo):
    G = nx.Graph()  # Cria um grafo vazio
    try:
        with open(arquivo, 'r') as f:
            dados = f.read().strip().split()  # Lê os números do arquivo
            # Converte os dados para inteiros e cria pares de arestas
            arestas = [(int(dados[i]), int(dados[i + 1])) for i in range(0, len(dados), 2)]
            G.add_edges_from(arestas)  # Adiciona as arestas ao grafo
    except FileNotFoundError:
        print(f"Arquivo '{arquivo}' não encontrado.")
    except ValueError:
        print("Erro ao processar o arquivo. Verifique o formato dos dados.")
    return G

# Nome do arquivo com as arestas
arquivo = "arquivo_teste_n10000.txt"

# Carrega o grafo a partir do arquivo
grafo = carregar_grafo(arquivo)

# Exibe as arestas e nós do grafo
#print("Arestas do grafo:", len(grafo.edges()))
#print("Nós do grafo:", len(grafo.nodes()))


def draw_net(G, pos, measures, measure_name):
    plt.figure(figsize=(9, 9))
    nodes = nx.draw_networkx_nodes(G, pos, cmap=plt.cm.plasma,
                                   node_color= list(measures),
                                   edgecolors= 'black',
                                   nodelist= measures.keys(),
                                   node_size = list(measures))
    nodes.set_norm(mcolors.SymLogNorm(linthresh=0.01, linscale=1, base=10))
    # labels = nx.draw_networkx_labels(G, pos)
    edges = nx.draw_networkx_edges(G, pos)

    plt.title(measure_name)
    plt.colorbar(nodes, location = 'bottom', fraction=0.046, pad=0.04)
    plt.axis('off')
    plt.savefig('rede.png')
    plt.show()

# Exemplo: Desenhar o grafo usando Matplotlib (opcional)
try:
    import matplotlib.pyplot as plt
    import pandas as pd
    import numpy as  np 
    grau = np.array([a[1] for a in nx.degree(grafo)])

    nx.draw_networkx(grafo,
                    #pos = pos,
                    cmap = 'inferno',
                    with_labels = False,
                    alpha = 0.9 ,
                    edge_color = '#1c1c17',
                    node_color = grau,
                    node_size = grau,
                    )
    plt.savefig("network_plot.png", format="png", dpi=300)  # Change format and dpi as needed
    plt.close()  # Close the plot to free up memory
except ImportError:
    print("Matplotlib não está instalado. Pulei a visualização do grafo.")
