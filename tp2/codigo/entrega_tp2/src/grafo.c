#include "grafo.h"
#include <stdlib.h> 
#include <stdio.h>
#include <limits.h>


//FUNÇÃO CONSTRUTOR DO TAD GRAFO
Grafo* faz_grafo(int n){ 
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
                       
    g->tamanho = n;
    g->grafo_vazio = 1; 
    g->matriz = (int*)malloc(sizeof(int*)*n*n); 


    for(int i=0; i<n*n; i++){
        g->matriz[i] = 0;
    }

    return g;
}


//CASO QUEIRA INSERIR O VALOR 3 NA ARESTA {1,2},
//USE insere_aresta(*seu_grafo, 1, 2);
void insere_aresta(Grafo *g, int v1, int v2, int peso){
    
    g->matriz[v1*g->tamanho + v2] = peso; 
    g->matriz[v2*g->tamanho + v1] = peso; 

    if(g->grafo_vazio == 1){
        g->grafo_vazio = 0; 
    }
}

//RETORNA 1 CASO EXISTA A ARESTA {v1, v2}
//DEFINIMOS COMO EXISTÊNCIA QUALQUER VALOR 
//DIFERENTE DE ZERO EM grafo[1,2]
int existe_aresta(Grafo *g, int v1, int v2){
    if(g->matriz[v1*g->tamanho + v2] != 0){
        return 1;
    }
}

//ADICIONA O VALOR 0 À ARESTA, FALSEANDO 
//A CONDIÇÃO DE EXISTENCIA POR NOS DEFINIDA

void retira_aresta(Grafo* g, int v1, int v2){
    g->matriz[v1*g->tamanho + v2] = 0; 
}

//PASSA POR TODOS OS VALORES DA MATRIZ DE ADJACÊNCIA 
//E IMPRIME OS VALORES ENCONTRADOS EM DISPOSIÇÃO TIPICAMENTE MATRICIAL
void imprime_grafo(Grafo *g){
    for(int i =0; i< g->tamanho; i++){
        for(int j=0; j< g->tamanho; j++){
            printf("%d\t", g->matriz[i *g->tamanho+j]);
        }
        printf("\n");
    }
}

//PROCURA O MENOR ELEMENTO PRESENTE NA MATRIZ DE ADJACÊNCIA
//HÁ DE SE TER CUIDADO POIS A CONDIDAÇÃO SÓ É SATISFEITA 
//PARA O MENOR NÚMERO DIFERENTE DE ZERO
int retira_min(Grafo *g){
   int min = INT_MAX; 

   for(int i =0; i<g->tamanho; i++){
       for(int j =0; j<g->tamanho; j++){
           if(g->matriz[i *g->tamanho + j] <= min && g->matriz[i*g->tamanho + j] != 0){
               min = g->matriz[i*g->tamanho + j];
           }
       }
   }
    return min; 
}

//LIBERA-SE PRIMEIRO OS N PONTEIROS QUE MAPEIAM OS VETORES DA MATRIZ
//EM SEGUIDA O PONTEIRO DA DA MATRIZ 
//E EM SEGUIDA O VETOR DO PRÓPRIO GRAFO
void libera_grafo(Grafo* g){
    free(g->matriz); 
    free(g);
}

//FUNÇÃO PARA VERIFICAR SE EXISTE ELEMENTOS ADJACENTES A DETERMINADO VERTICE
//A FUNCAO PERCORRE TODAS AS COLUNAS AO QUAL AQUELE VERTICE SE CONCECTA
//SE ENCONTRA ALGO DIFERENTE DE ZERO RETORNA FALSE (listaVazia == 0)
//SE NÃO ENCONTRA NADA DIFERENTE DE ZERO EM TODOS OS ELEMENTOS, RETRNA TRUE(listaVAzia ==1) 
int lista_adj_vazia(Grafo *g, int v){
    int numVertices = (g->tamanho)*(g->tamanho); 
    int aux = 0; 
    int listaVazia = 1; 

    while(aux < g->tamanho && listaVazia == 1){
        if(g->matriz[v*g->tamanho + aux] != 0){
            listaVazia = 0;
        }
        else{
            aux++;
        }
    }
    
    return listaVazia; 

}

int* obtem_lista_vertices_adj(Grafo* g, int u, int *tam) {
    // Verifica se o grafo está vazio ou se o vértice está fora do limite
    if (g->grafo_vazio || u < 0 || u >= g->tamanho) {
        return NULL;
    }

    // Conta o número de vértices adjacentes a u
    int count = 0;
    for (int v = 0; v < g->tamanho; v++) {
        if (g->matriz[u*g->tamanho + v] != 0) {
            count++;
        }
    }

    // Aloca um array para armazenar os vértices adjacentes
    int* adjacentes = (int*)malloc(count * sizeof(int));
    if (adjacentes == NULL) {
        return NULL; // Falha na alocação
    }

    // Armazena os vértices adjacentes no array
    int idx = 0;
    for (int v = 0; v < g->tamanho; v++) {
        if (g->matriz[u*g->tamanho + v] != 0) {
            adjacentes[idx++] = v;
        }
    }

    *tam = count;
    return adjacentes;
}
