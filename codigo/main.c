#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"



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
    adj_list = obtem_lista_vertices_adj(g, 7); 
    printf("Adjacentes:\n");
    for (int i =0; i< sizeof(adj_list)/sizeof(int); i++){
        printf("%d\t", adj_list[i]);
    }
    printf("\n \b");

    return 0; 
}