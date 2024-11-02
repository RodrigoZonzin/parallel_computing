#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

void printa_vetor(int* vet, int tam){
    for(int i=0; i<tam; i++){
        printf("%d", vet[i]);
    }
    printf("\n");
}


int* determina_vizinhos(Grafo *g, int u, int v){
    int nu, nv;

    int *adj_u = obtem_lista_vertices_adj(g, u, &nu), 
        *adj_v = obtem_lista_vertices_adj(g, v, &nv);  

    printf("Vetores: \n");
    printa_vetor(adj_u, nu);
    printa_vetor(adj_v, nv);

    // |UV| <= max(|U|, |V|)  
    //a cardinalidade do conjunto intersecção é, no máximo, igual a maior dos outros dois conjuntos
    int maxNuNv = (nu > nv) ? nu : nv;
    printf("Nu: %d Nv: %d Max: %d\n", nu, nv, maxNuNv); 
    int *intersec = (int*)malloc(sizeof(int)*maxNuNv);

    int k = 0;
    for(int i = 0; i < nu; i++){
        for(int j = 0; j< nv; j++){
            
            if(adj_u[i] == adj_v[j]){
                intersec[k] = adj_u[i];
                k++;
                break;
            }
        }
    }

    printf("Resultado Intersecção: \n");
    for(int i=0; i< maxNuNv; i++){
        printf("%d\t", intersec[i]);
    }
}

int main(int argc, char **argv){

    char nome_arquivo[100];  
    int *adj_list; 
    strcpy(nome_arquivo, argv[1]);
    Grafo *g = faz_grafo(10);

    int c1, c2; 
    FILE *f = fopen(nome_arquivo, "r"); 

    while (fscanf(f, "%d %d", &c1, &c2) == 2){
        insere_aresta(g, c1, c2, 1); 
    }

    fclose(f); // Close the file

    imprime_grafo(g);
    

    determina_vizinhos(g, 5, 8);
    return 0; 
}