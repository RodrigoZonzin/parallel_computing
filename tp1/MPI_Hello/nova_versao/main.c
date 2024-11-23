#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "Grafo.h"

#define DEBUGA printf("Erro aqui %d\n", CHAMADA); CHAMADA++;
int CHAMADA; 

char* novo_nome_arquivo(char* nome_arquivo){
    char* ponto = strrchr(nome_arquivo, '.');
    
    size_t tam = ponto - nome_arquivo;
    char* novo_nome = (char*)malloc(tam + strlen(".cng") + 1); 
    strncpy(novo_nome, nome_arquivo, tam);
    novo_nome[tam] = '\0';  
    
    strcat(novo_nome, ".cng");
    return novo_nome;
}

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

int determina_vizinhos(int* v1, int* v2, int n){
    int k = 0; 
    
    for(int i=0; i<n;i++){
        if(v1[i] == v2[i] && (v1[i] != 0 || v2[i] != 0)) k++;
    }

    return k;
}

//slice_vetor(vet, k, m) === vet[k:m]
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

int main(int argc, char** argv){
    int my_rank, num_clusters;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Código mestre
    if(my_rank == 0){
        char nome_arquivo[100];  
        strcpy(nome_arquivo, argv[1]);
        char* novo_nome = novo_nome_arquivo(nome_arquivo);

        FILE *f = fopen(nome_arquivo, "r"); 
        FILE *f_saida = fopen(novo_nome_arquivo(nome_arquivo), "w");

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
        
        //cria grafo com o numero correto de vertices
        Grafo *g = faz_grafo(n);

        rewind(f);
        while(fscanf(f, "%d %d", &caractere1, &caractere2) == 2){
            insere_aresta(g, caractere1, caractere2, 1); 
        }
        fclose(f);

        // Envia o tamanho do subgrafo
        //tamanho do subgrafo será tamanho do grafo/numero de computadores
        //int sub_n = g->tamanho/num_clusters;
        int sub_n = g->tamanho*g->tamanho/2;
        MPI_Bcast(&g->tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);


        // Envia a matriz de adjacência como um array linear
        //subgrafo 1 vai das linhas 0 até sub_n e subgrafo2 de sub_n até g->tamanho

        imprime_grafo(g); 

        int nsub1, nsub2; 
        int *subgrafo1 = slice_vetor(g->matriz, 0, sub_n, &nsub1); 
        int *subgrafo2 = slice_vetor(g->matriz, sub_n, g->tamanho*g->tamanho, &nsub2);

        int tamanhos[] = {nsub1, nsub2};
        MPI_Send(&tamanhos, 2, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        MPI_Send(subgrafo1, nsub1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(subgrafo2, nsub2, MPI_INT, 1, 1, MPI_COMM_WORLD);

        free(g->matriz);
        free(g);
    } 
    
    //Código obediente
    else if(my_rank > 0){
        int tamanho; 
        int *tamanhos = (int*)malloc(sizeof(int)*num_clusters);

        MPI_Bcast(&tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Aloca memória para receber a matriz de adjacência
        MPI_Recv(&tamanhos, 2, MPI_INT, 0, MPI_COMM_WORLD);
        int *linha1 =(int*)malloc(sizeof(int)*tamanho);
        int *linha3 =(int*)malloc(sizeof(int)*tamanho);


        // Recebe a matriz de adjacência
        MPI_Recv(linha1, tamanhos[0], MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(linha3, tamanhos[1], MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //printa_vetor(linha1, tamanho); 
        //printa_vetor(linha3, tamanho); 


        int k, sub_n;
        /*for(int i = 0; i<sub_n; i++){
            for(int j = 0; j<sub_n;j++){
                k = determina_vizinhos()
            }
        }
        

        k = determina_vizinhos(linha1 , linha3, tamanho);
        printf("Tamanho: %d\n", k);
        */

        free(linha1);
        free(linha3);
    }

    MPI_Finalize();
    return 0;
}