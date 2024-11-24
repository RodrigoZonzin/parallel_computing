#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "Utilitarios.h"
#include "Grafo.h"

#define DEBUGA printf("Erro aqui %d\n", CHAMADA); CHAMADA++;
int CHAMADA; 


int main(int argc, char** argv){
    int my_rank, num_clusters;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Código mestre
    if(my_rank == 0){
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

        imprime_grafo(g);
        /* COMECO DA TROCA DE MENSAGENS*/

        // Envia o tamanho do subgrafo
        //tamanho do subgrafo será tamanho do grafo/numero de computadores
        MPI_Bcast(&g->tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);


        // Envia a matriz de adjacência como um array linear
        //subgrafo 1 vai das linhas 0 até sub_n e subgrafo2 de sub_n até g->tamanho

        int nsub1, nsub2; 
        int sub_n = g->tamanho * (g->tamanho/num_clusters);        //numero de linhas * tamanho do grafo

        int *subgrafo1 = slice_vetor(g->matriz, 0, sub_n, &nsub1); 
        int *subgrafo2 = slice_vetor(g->matriz, sub_n, g->tamanho*g->tamanho, &nsub2);

        int tamanhos[] = {nsub1, nsub2};
        MPI_Send(&tamanhos, 2, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        MPI_Send(subgrafo1, nsub1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(subgrafo2, nsub2, MPI_INT, 1, 1, MPI_COMM_WORLD);

        fclose(f);
        free(vetor);
        free(novo_nome);
        free(g->matriz);
        free(g);
    } 
    
    //Código obediente
    else if(my_rank > 0){
        int g_tam; 
        int *tamanhos = (int*)malloc(sizeof(int)*num_clusters);

        MPI_Bcast(&g_tam, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Aloca memória para receber a matriz de adjacência
        MPI_Recv(tamanhos, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int *linha1 =(int*)malloc(sizeof(int)*tamanhos[0]);
        int *linha3 =(int*)malloc(sizeof(int)*tamanhos[1]);


        // Recebe a matriz de adjacência
        MPI_Recv(linha1, tamanhos[0], MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(linha3, tamanhos[1], MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printa_vetor(linha1, tamanhos[0]);
        printa_vetor(linha3, tamanhos[1]);

        int k = 0; 
        for(int i =0; i< tamanhos[0]/g_tam; i++){
            for(int j=0; j<g_tam; j++){
                printf("%-5d ", linha1[i*g_tam +j]);                
            }
        }
    


        free(linha1);
        free(linha3);
    }

    MPI_Finalize();
    return 0;
}