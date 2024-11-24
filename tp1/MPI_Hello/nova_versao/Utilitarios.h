#define UTILITARIOS_H
#include <stdio.h>
//#include "Grafo.h"

/*A funao desse codigo eh reunir algumas funcoes 
e despoluir a main*/

/*Retira o .txt, .csv etc de um arquivo e add .cng*/
char* novo_nome_arquivo(char* nome_arquivo){
    char* ponto = strrchr(nome_arquivo, '.');
    
    size_t tam = ponto - nome_arquivo;
    char* novo_nome = (char*)malloc(tam + strlen(".cng") + 1); 
    strncpy(novo_nome, nome_arquivo, tam);
    novo_nome[tam] = '\0';  
    
    strcat(novo_nome, ".cng");
    return novo_nome;
}

/*PRinta um vetor de inteiros na forma 1 2 3 ... n*/
void printa_vetor(int* vet, int tam){
    for(int i=0; i<tam; i++){
        printf("%d ", vet[i]);
    }
    printf("\n");
}

/*Dado uma sequencia (a1, a2, ..., an), determina quantos numeros ai 
não sao repetidos. Ex (1, 1, 1, 3) retorna 2 pois unitarios sao 1 e 3*/
int num_diferentes(int* vet, int tam){
    int num = 0;
    
    int encontrado[tam];
    for (int i = 0; i < tam; i++){
        encontrado[i] = 0; 
    }
    
    //Verifica elementos únicos
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


/*Seja Vu o vetor de adjacencia do vertice u e Vv, do vertice v
essa funcao retorna o modulo do conjunto intersecao entre Vu e Vv
Vu = [1 1 0 1] e Vv = [1 0 1 0] => VuVv = [1 0 0 0] => |VuVv| = 1*/
int determina_vizinhos(int* v1, int* v2, int n){
    int k = 0; 
    
    for(int i=0; i<n;i++){
        if(v1[i] == v2[i] && (v1[i] != 0 || v2[i] != 0)) k++;
    }

    return k;
}

//slice_vetor(vet, k, m) === vet[k:m], como no Python
int* slice_vetor(int *vet, int k, int m, int *tam_retorno){
    int *novo_vetor =(int*)malloc(sizeof(int)*(m-k)); 

    int j = 0; 
    for(int i=k;i<m; i++){
        novo_vetor[j] = vet[i]; 
        j++; 
    }
    *tam_retorno = j;
    return novo_vetor;
}