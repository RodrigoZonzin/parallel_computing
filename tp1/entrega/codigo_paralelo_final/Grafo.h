#define _GRAFO_H

#include <stdlib.h>
#include <stdio.h>

struct grafo {
    int tamanho;
    int* matriz;  // Matriz de adjacência linearizada
};

typedef struct grafo Grafo;

Grafo* faz_grafo(int); 
void insere_aresta(Grafo*, int, int, int); 
int determina_vizinhos(int*, int*, int); 

