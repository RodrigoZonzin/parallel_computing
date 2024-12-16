#ifndef GRAFO_H
#define GRAFO_H


struct grafo{
    int tamanho;        //O GRAFO TERÁ, NA VERDADE, tamanho^2 TAMANHO
    int grafo_vazio;    //==1 sse O GRAFO ESTÁ VAZIO
    int **matriz;  
};
typedef struct grafo Grafo; 

Grafo* faz_grafo(int); 
void insere_aresta(Grafo*, int, int, int); 
int existe_aresta(Grafo*, int, int);
int* obtem_lista_vertices_adj(Grafo*, int, int*);
void retira_aresta(Grafo*, int, int); 
void libera_grafo(Grafo*); 
void imprime_grafo(Grafo*); 
int retira_min(Grafo*);
int lista_adj_vazia(Grafo*, int);


#endif