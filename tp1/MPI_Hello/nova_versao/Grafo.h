#define GRAFO_H

struct grafo{
    int tamanho;  // Tamanho da matriz (número de vértices)
    int* matriz;  // Matriz de adjacência representada como array linear
};

typedef struct grafo Grafo;

Grafo* faz_grafo(int);
void insere_aresta(Grafo*, int, int, int); 