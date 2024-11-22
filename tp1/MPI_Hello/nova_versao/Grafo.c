#include "Grafo.h"
#include <stdlib.h>
#include <string.h>

Grafo* faz_grafo(int n){
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->tamanho = n;
    g->matriz = (int*)malloc(n * n * sizeof(int));
    memset(g->matriz, -1, n * n * sizeof(int)); 
    return g;
}

void insere_aresta(Grafo* g, int u, int v, int peso){
    g->matriz[u * g->tamanho + v] = peso;
    g->matriz[v * g->tamanho + u] = peso;  
}