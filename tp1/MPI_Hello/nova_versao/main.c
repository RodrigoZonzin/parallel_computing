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

int determina_vizinhos(int *linha1, int n1, int *linha2, int n2, int *return_k){
    int nu = n1,
        nv = n2;


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
            
            if(linha1[i] == linha2[j]){
                intersec[k] = linha1[i];
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

    printa_vetor(intersec, maxNuNv);

    *return_k = k; 
    return k;
}

int main(int argc, char** argv){
    int my_rank, num_clusters;
    int u = atoi(argv[2]),
        v = atoi(argv[3]);

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

        printa_vetor(vetor, tam);
        int n = num_diferentes(vetor, tam); 
        free(vetor);

        Grafo *g = faz_grafo(n);

        rewind(f);
        while(fscanf(f, "%d %d", &caractere1, &caractere2) == 2){
            insere_aresta(g, caractere1, caractere2, 1); 
        }

        fclose(f);


        // Envia o tamanho do grafo
        MPI_Bcast(&g->tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int *linha1 = &g->matriz[3*g->tamanho];
        int *linha3 = &g->matriz[4*g->tamanho];
        
        // Envia a matriz de adjacência como um array linear
        MPI_Send(linha1, g->tamanho, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(linha3, g->tamanho, MPI_INT, 1, 1, MPI_COMM_WORLD);

        free(g->matriz);
        free(g);
    } 
    
    //Código obediente
    else if(my_rank > 0){
        int tamanho;
        MPI_Bcast(&tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Aloca memória para receber a matriz de adjacência
        //int* matriz =(int*)malloc(tamanho * tamanho * sizeof(int));
        int *linha1 =(int*)malloc(sizeof(int)*tamanho);
        int *linha3 =(int*)malloc(sizeof(int)*tamanho);


        // Recebe a matriz de adjacência
        MPI_Recv(linha1, tamanho, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(linha3, tamanho, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int k;
        k = determina_vizinhos(linha1, tamanho, linha3, tamanho, &k);
        printf("Tamanho: %d\n", k);

        free(linha1);
        free(linha3);
    }

    MPI_Finalize();
    return 0;
}