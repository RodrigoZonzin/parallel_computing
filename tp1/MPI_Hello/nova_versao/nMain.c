#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "Utilitarios.h"
#include "Grafo.h"


int main(int argc, char** argv){
    int rank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int gtam = 0; 


    FILE *f = fopen(argv[1], "r"); 
    char* novo_nome = novo_nome_arquivo(argv[1]);
    FILE *f_saida = fopen(novo_nome_arquivo(argv[1]), "w");


    //variaveis para ler a sequencia de inteiros do arquivo (1 1 2 3 ...)
    int tam = 0, 
        caractere1 = 0;     
    
    //contando quantos elementos estão presentes no arquivo de entrada      
    while(!feof(f)){
        fscanf(f, "%d", &caractere1);
        tam++;    
    }

    //alocando um vetor para armazenar a sequencia
    int *vetor = (int*)malloc(sizeof(int)*tam);
    
    //move o ponteiro de leitura do arquivo para o comeco
    //lê novamente para armazenar 
    rewind(f); 
    int j = 0; 
    while(!feof(f)){
        fscanf(f, "%d", &caractere1);
        vetor[j] = caractere1; 
        j++;
    }

    //retorna o numero de elementos unitarios na sequencia
    //equivalente a fazer set([1, 2, 1, 4, 2]) = {1, 2, 4} no Python e pegar sua dimensao
    int n = num_diferentes(vetor, tam); 

    //cria grafo com o numero correto de vertices
    Grafo *g = faz_grafo(n);
    
    //inserindo as arestas (a1, a2), (a3, a4) no grafo 'g'
    for(int i = 0; i<tam; i++){
        insere_aresta(g, vetor[i], vetor[++i], 1); 
    }

    
    int numero_dados_por_processo = (g->tamanho * g->tamanho) / nprocs;
    //rank 1 vai receber todos os dados em processos desquilibrados
    if(rank == 1){
        numero_dados_por_processo += (g->tamanho * g->tamanho % nprocs);         
    }

    int *recvbuf = (int*)malloc(sizeof(int)*numero_dados_por_processo); 

    /*
    int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
            void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
            MPI_Comm comm)
    */
    //MPI_Scatter(&g->matriz, numero_dados_por_processo, MPI_INT, recvbuf, numero_dados_por_processo, MPI_INT, 0, MPI_COMM_WORLD); 
    MPI_Scatter(g->matriz, numero_dados_por_processo, MPI_INT, recvbuf, numero_dados_por_processo, MPI_INT, 0, MPI_COMM_WORLD);


    /*
    printf("In rank %d:\n", numero_dados_por_processo);
    for(int i =0; i<numero_dados_por_processo; i++){
        printf("%d ", recvbuf[i]);
    }
    printf("\n");
    */

    if(rank == 0){
        int *vet_1 = (int*)malloc(sizeof(int)*gtam);
        int *vet_2 = (int*)malloc(sizeof(int)*gtam);
        printf("Grafo reconstruido:\n");
        for(int i =0; i < (numero_dados_por_processo/g->tamanho) -1;i++){
            for(int j =0; j< g->tamanho; j++){
                vet_1[j] = recvbuf[i*g->tamanho +j];
            }
            printf("\t");
            for(int j =0; j< g->tamanho; j++){
                vet_2[j] = recvbuf[(i+1)*g->tamanho +j];
            }
            printf("\n");
        }
    }

    MPI_Finalize();
}