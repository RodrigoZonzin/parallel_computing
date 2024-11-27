#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

struct grafo {
    int tamanho;
    int* matriz;  // Matriz de adjacência linearizada
};

typedef struct grafo Grafo;

Grafo* faz_grafo(int n) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->tamanho = n;
    g->matriz = (int*)calloc(n * n, sizeof(int));
    return g;
}

void insere_aresta(Grafo* g, int u, int v, int peso) {
    g->matriz[u * g->tamanho + v] = peso;
    g->matriz[v * g->tamanho + u] = peso;
}

int determina_vizinhos(int* v1, int* v2, int n) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (v1[i] == 1 && v2[i] == 1) k++;
    }
    return k;
}

void calcula_vizinhos_comuns_paralelo(Grafo* g, int my_rank, int num_clusters, FILE* f_saida) {
    int n = g->tamanho;
    int total_pares = n * (n - 1) / 2; // Número de pares u, v (u < v)
    int pares_por_processo = total_pares / num_clusters;
    int inicio = my_rank * pares_por_processo;
    int fim = (my_rank == num_clusters - 1) ? total_pares : inicio + pares_por_processo;

    // Cálculo local dos pares
    for (int idx = inicio; idx < fim; idx++) {
        int u = 0, v = 0, contador = 0;

        // Encontrar o par correspondente ao índice
        for (u = 0; u < n; u++) {
            for (v = u + 1; v < n; v++) {
                if (contador == idx) goto fim_loop;
                contador++;
            }
        }
    fim_loop:
        

        // Obter os vetores de adjacência
        int* vu = &g->matriz[u * n];
        int* vv = &g->matriz[v * n];
        int vizinhos_comuns = determina_vizinhos(vu, vv, n);
        if(vizinhos_comuns == 0) continue;
        // Escrever resultados no arquivo de saída
        fprintf(f_saida, "%d %d %d\n", u, v, vizinhos_comuns);
    }
}

int main(int argc, char** argv) {
    int my_rank, num_clusters;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (argc != 2) {
        if (my_rank == 0) {
            fprintf(stderr, "Uso: %s <arquivo_edgelist>\n", argv[1]);
        }
        MPI_Finalize();
        return 1;
    }

    Grafo* g = NULL;
    FILE* f_saida = NULL;

    if (my_rank == 0) {
        // Mestre: lê o grafo do arquivo
        FILE* f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Erro ao abrir arquivo %s\n", argv[1]);
            MPI_Finalize();
            return 1;
        }

        int u, v;
        int max_vertice = -1;

        // Descobrir número de vértices
        while (fscanf(f, "%d %d", &u, &v) != EOF) {
            if (u > max_vertice) max_vertice = u;
            if (v > max_vertice) max_vertice = v;
        }
        rewind(f);

        g = faz_grafo(max_vertice + 1);

        // Inserir arestas
        while (fscanf(f, "%d %d", &u, &v) != EOF) {
            insere_aresta(g, u, v, 1);
        }
        fclose(f);

        // Cria arquivo de saída
        f_saida = fopen("saida.txt", "w");
    }

    // Envia grafo para outros processos
    int n;
    if (my_rank == 0) n = g->tamanho;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank != 0) {
        g = faz_grafo(n);
    }
    MPI_Bcast(g->matriz, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Abre o arquivo de saída para escrita
    if (my_rank != 0) {
        f_saida = fopen("saida.txt", "a");
    }

    // Cálculo paralelo
    calcula_vizinhos_comuns_paralelo(g, my_rank, num_clusters, f_saida);

    fclose(f_saida);
    free(g->matriz);
    free(g);

    MPI_Finalize();
    return 0;
}
