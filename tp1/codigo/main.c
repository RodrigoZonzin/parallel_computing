#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

void printa_vetor(int* vet, int tam){
    for(int i=0; i<tam; i++){
        printf("%d ", vet[i]);
    }
    printf("\n");
}


int num_diferentes(int* vet, int tam){
    int num = 0;
    
    int encontrado[tam];
    for (int i = 0; i < tam; i++){
        encontrado[i] = 0; 
    }
    
    // Verifica elementos únicos
    for(int i = 0; i < tam; i++){
        int repetido = 0;
        for(int j = 0; j < num; j++){
            if(vet[i] == encontrado[j]){
                repetido = 1;
                break;
            }
        }
        if(!repetido){
            encontrado[num] = vet[i];
            num++;
        }
    }
    
    return num;
}


int* determina_vizinhos(Grafo *g, int u, int v, int *return_size, int *return_k){
    int nu, nv;

    int *adj_u = obtem_lista_vertices_adj(g, u, &nu), 
        *adj_v = obtem_lista_vertices_adj(g, v, &nv);  


    // |UV| <= max(|U|, |V|)  
    //a cardinalidade do conjunto intersecção é, no máximo, igual a maior dos outros dois conjuntos
    int maxNuNv = (nu > nv) ? nu : nv;
    int *intersec = (int*)malloc(sizeof(int)*maxNuNv);
    for(int i =0; i<maxNuNv; i++){
        intersec[i] = -1;
    }

    //determina os vizinhos
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
    
    //determina o número de vizinhos
    k = 0;
    for(int i = 0; i< maxNuNv; i++){
        if(intersec[i] >= 0) k++;
    }

    *return_size = maxNuNv;
    *return_k = k; 
    return intersec;
}

int mainAntiga(int argc, char **argv){

    char nome_arquivo[100];  
    int *adj_list; 
    strcpy(nome_arquivo, argv[1]);
    Grafo *g = faz_grafo(10);

    int c1, c2; 
    FILE *f = fopen(nome_arquivo, "r"); 

    while(fscanf(f, "%d %d", &c1, &c2) == 2){
        insere_aresta(g, c1, c2, 1); 
    }
    
    fclose(f); // Close the file
    imprime_grafo(g);

    
    return 0; 
}

int main(int argc, char **argv){
    char nome_arquivo[100];  
    strcpy(nome_arquivo, argv[1]);

    FILE *f = fopen(nome_arquivo, "r"); 

    //contando quantos elementos estão presentes no arquivo de entrada
    int caractere1, caractere2; 
    int tam = 0; 

    while(!feof(f)){
        fscanf(f, "%d", &caractere1);
        tam++;    
    }

    int *vetor = (int*)malloc(sizeof(int)*tam);

    //move o ponteiro de leitura do arquivo para o comeco
    rewind(f); int j = 0; 

    //armazena as informações em vetor para verificar quantos elementos unitarios existem
    while(!feof(f)){
        fscanf(f, "%d %d", &caractere1, &caractere2);
        vetor[j] = caractere1; vetor[++j] = caractere2; 
        printf("%d %d\n", caractere1, caractere2);
        
        j++;
    }

    int n = num_diferentes(vetor, tam); 
    free(vetor);
    
    Grafo *g = faz_grafo(n);

    rewind(f);
    while(fscanf(f, "%d %d", &caractere1, &caractere2) == 2){
        insere_aresta(g, caractere1, caractere2, 1); 
    }

    imprime_grafo(g);

    for(int i =0; i< g->tamanho; i++){
        
        int modulo_nu, k; 
        int *nu = obtem_lista_vertices_adj(g, i, &modulo_nu);
        
        for(int j = 0; j < g->tamanho; j++){
            if(i == j) continue;
            int *intersec; 
            int N_intersec = 0; 
            intersec = determina_vizinhos(g, i, j, &N_intersec, &k);
            
            if(k <= 0) continue;
            printf("%d %d %d\n", i, j, k);
            
            
            //printa_vetor(intersec, N_intersec);
            
        }
    }

    fclose(f);
    return 0;
}