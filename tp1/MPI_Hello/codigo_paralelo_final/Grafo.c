#include "Grafo.h"

Grafo* faz_grafo(int n){
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->tamanho = n;
    g->matriz = (int*)calloc(n * n, sizeof(int));
    return g;
}

void insere_aresta(Grafo* g, int u, int v, int peso) {
    g->matriz[u * g->tamanho + v] = peso;
    g->matriz[v * g->tamanho + u] = peso;
}

int determina_vizinhos(int* v1, int* v2, int n){
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (v1[i] == 1 && v2[i] == 1) k++;
    }
    return k;
}