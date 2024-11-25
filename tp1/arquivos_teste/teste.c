#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

// Função fictícia para simular a estrutura Grafo
typedef struct {
    int tamanho;  // Tamanho da matriz (número de vértices)
    int* matriz;  // Matriz de adjacência representada como array linear
} Grafo;

// Simula a criação do grafo (substitua conforme necessário)
Grafo* faz_grafo(int n) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->tamanho = n;
    g->matriz = (int*)malloc(n * n * sizeof(int));
    memset(g->matriz, 0, n * n * sizeof(int));  // Inicializa com 0
    return g;
}

// Simula a inserção de aresta no grafo
void insere_aresta(Grafo* g, int u, int v, int peso) {
    g->matriz[u * g->tamanho + v] = peso;
    g->matriz[v * g->tamanho + u] = peso;  // Considerando grafo não dirigido
}

// Função para imprimir o vetor
void printa_vetor(int* vetor, int tam) {
    for (int i = 0; i < tam; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    int my_rank, num_clusters;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {
        // Código mestre
        int n = 4;  // Número de vértices do grafo (exemplo)
        Grafo* g = faz_grafo(n);

        // Simula a construção do grafo
        insere_aresta(g, 0, 1, 1);
        insere_aresta(g, 1, 2, 1);
        insere_aresta(g, 2, 3, 1);
        insere_aresta(g, 3, 0, 1);

        // Envia o tamanho do grafo
        MPI_Bcast(&g->tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int *linha1 = &g->matriz[0*g->tamanho];
        int *linha3 = &g->matriz[2*g->tamanho];
        
        // Envia a matriz de adjacência como um array linear
        MPI_Send(linha1, g->tamanho, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(linha3, g->tamanho, MPI_INT, 1, 1, MPI_COMM_WORLD);

        free(g->matriz);
        free(g);
    } 
    
    
    else if (my_rank == 1) {
        // Código escravo
        int tamanho;
        MPI_Bcast(&tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Aloca memória para receber a matriz de adjacência
        //int* matriz = (int*)malloc(tamanho * tamanho * sizeof(int));
        int *linha1 = (int*)malloc(sizeof(int)*tamanho);
        int *linha3 = (int*)malloc(sizeof(int)*tamanho);


        // Recebe a matriz de adjacência
        MPI_Recv(linha1, tamanho, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(linha3, tamanho, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printa_vetor(linha1, tamanho);
        printa_vetor(linha3, tamanho);

        free(linha1);
        free(linha3);
    }

    MPI_Finalize();
    return 0;
}

