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

// retorna o numero de elementos unitarios em vet 
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


char* novo_nome_arquivo(char* nome_arquivo){
    char* ponto = strrchr(nome_arquivo, '.');
    

    // Criar uma cópia do nome do arquivo até o ponto (sem a extensão)
    size_t tam = ponto - nome_arquivo;
    char* novo_nome = (char*)malloc(tam + strlen(".cng") + 1); 
            
    //copia nome do arquivo sem extensao
    strncpy(novo_nome, nome_arquivo, tam);
    novo_nome[tam] = '\0';  
    
    // Adicionar a nova extensão ".cng"
    strcat(novo_nome, ".cng");
    
    return novo_nome;
}

int determina_vizinhos(Grafo* g, int u, int v){
    int n = g->tamanho;
    int k = 0; 

    for(int j = 0; j<n; j++){
        k += g->matriz[u][j] && g->matriz[v][j];
    }
    
    return k;
}

int main(int argc, char **argv){
    char* novo_nome = novo_nome_arquivo(argv[1]);
    
    FILE *f = fopen(argv[1], "r"); 
    FILE *f_saida = fopen(novo_nome_arquivo(argv[1]), "w");

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
        
        j++;
    }

    int n = num_diferentes(vetor, tam); 
    free(vetor);
    
    Grafo *g = faz_grafo(n);

    rewind(f);
    while(fscanf(f, "%d %d", &caractere1, &caractere2) == 2){
        insere_aresta(g, caractere1, caractere2, 1); 
    }


    for(int i =0; i< g->tamanho; i++){
        int k;
        
        for(int j = 0; j < g->tamanho; j++){

            //imprime só a matriz triangular inferior
            if(i >= j) continue;

            //determina o modulo do conjunto N(u)(intesec)N(v)
            k = determina_vizinhos(g, i, j);
            
            //se o número de vizinhos for inferior a 1, pula
            if(k <= 0) continue;

            fprintf(f_saida, "%d %d %d\n", i, j, k);
        }
    }

    fclose(f_saida);
    fclose(f);
    return 0;
}