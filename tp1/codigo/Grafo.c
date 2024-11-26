#include "Grafo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Grafo* faz_grafo(int n){
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->tamanho = n;
    g->matriz = (int*)malloc(n * n * sizeof(int));
    memset(g->matriz, 0, n * n * sizeof(int)); 
    return g;
}

void insere_aresta(Grafo* g, int u, int v, int peso){
    g->matriz[u * g->tamanho + v] = peso;
    g->matriz[v * g->tamanho + u] = peso;  
}

void imprime_grafo(Grafo *g){
    for(int i =0; i< g->tamanho; i++){
        for(int j = 0; j<g->tamanho; j++){
            printf("%-5d", g->matriz[i*g->tamanho + j]); 
        }
        printf("\n");
    }
}