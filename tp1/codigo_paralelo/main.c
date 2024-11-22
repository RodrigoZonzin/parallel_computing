#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include <mpi.h> 

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

    free(adj_u);
    free(adj_v);
    *return_size = maxNuNv;
    *return_k = k; 
    return intersec;
}

int main(int argc, char **argv){
    
    //configurando ambiente MPI
    int my_rank, num_clusters; 
    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD, &num_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //codigo mestre
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
        
        Grafo *g = faz_grafo(n);

        rewind(f);
        while(fscanf(f, "%d %d", &caractere1, &caractere2) == 2){
            insere_aresta(g, caractere1, caractere2, 1); 
        }

        fclose(f);

        //send vetor para o codigo obediente
        MPI_Send(&g->tamanho, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        //sincronizar
        MPI_Send(&g->matriz[0], g->tamanho, MPI_INT, 1, 1, MPI_COMM_WORLD);
        //MPI_Send(&g->matriz[1], g->tamanho, MPI_INT, 1, 1, MPI_COMM_WORLD);

        fclose(f_saida);
    }


    //codigo obediente
    if(my_rank == 1){
        int buf_size; 
        int *buffer; 

        MPI_Recv(&buf_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Tamanho do buffer recebido: %d\n", buf_size);

        //sincronizar 
        buffer = (int*)malloc(sizeof(int)*buf_size);
        printa_vetor(buffer, buf_size);
    }
    
    return 0;
}

