#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#include "Grafo.h"

/*Retira o .txt, .csv etc de um arquivo e add .cng*/
char* novo_nome_arquivo(char* nome_arquivo){
    char* ponto = strrchr(nome_arquivo, '.');
    
    size_t tam = ponto - nome_arquivo;
    char* novo_nome =(char*)malloc(tam + strlen(".cng") + 1); 
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

/*Dado uma sequencia(a1, a2, ..., an), determina quantos numeros ai 
não sao repetidos. Ex(1, 1, 1, 3) retorna 2 pois unitarios sao 1 e 3*/
int num_diferentes(int* vet, int tam){
    int num = 0;
    
    int encontrado[tam];
    for(int i = 0; i < tam; i++){
        encontrado[i] = 0; 
    }
    
    //determina elementos unicos para saber o tamanho de vertices a se alocar
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

void calcula_vizinhos_comuns_paralelo(Grafo* g, int my_rank, int num_clusters, FILE* f_saida){
    int n = g->tamanho;
    //numero maximo de pares em um grafo
    int pares_por_processo = (n *(n - 1)/2)  / num_clusters;
    int inicio = my_rank * pares_por_processo;
    
    int fim;
    if(my_rank == num_clusters - 1)
        fim = n *(n - 1)/2; 
    else
        fim = inicio + pares_por_processo;
    

    //calculo local dos pares
    for(int idx = inicio; idx < fim; idx++){
        int u = 0, v = 0, contador = 0;

        //encontrar o par correspondente ao indice
        for(u = 0; u < n; u++){
            for(v = u + 1; v < n; v++){
                if(contador == idx){
                    //break quebra o look de dentro
                    goto fim_loop;
                }
                contador++;
            }
        }
    fim_loop:
        //vetores de adjacência dos vertices u e v em G
        int* vu = &g->matriz[u * n];
        int* vv = &g->matriz[v * n];

        int vizinhos_comuns = determina_vizinhos(vu, vv, n);
        if(vizinhos_comuns == 0) continue;
        
        //append nos resultados
        fprintf(f_saida, "%d %d %d\n", u, v, vizinhos_comuns);
    }
}


int main(int argc, char** argv){
    int my_rank, num_clusters;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(argc != 2){
        if(my_rank == 0){
            perror("Faltou o arquivo\n");
        }
        MPI_Finalize();
        return 1;
    }

    Grafo* g = NULL;
    FILE* f_saida = NULL;

    if(my_rank == 0){
        //Codigo mestre
        FILE* f = fopen(argv[1], "r");
        if(!f){
            fprintf(stderr, "Erro ao abrir arquivo %s\n", argv[1]);
            MPI_Finalize();
            return 1;
        }

        int u, v;
        int max_vertice = -1;

        //descobrir numero de vertices
        while(fscanf(f, "%d %d", &u, &v) != EOF){
            if(u > max_vertice) max_vertice = u;
            if(v > max_vertice) max_vertice = v;
        }

        //move ponteiro de leitura p o comeco
        rewind(f);

        g = faz_grafo(max_vertice + 1);

        //inserir arestas
        while(fscanf(f, "%d %d", &u, &v) != EOF){
            insere_aresta(g, u, v, 1);
        }
        fclose(f);

        //cria arquivo de saida
        f_saida = fopen(novo_nome_arquivo(argv[1]), "w");
    }

    //envia grafo para outros processos
    int n;
    if(my_rank == 0){
        n = g->tamanho;
    }
    //recebe o bcast se rank != 0 e envia se rank = 0
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //todos os outros processos alocam o grafo
    if(my_rank != 0){
        g = faz_grafo(n);
    }
    //e recebem os dados
    MPI_Bcast(g->matriz, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    //abre o arquivo de saida para escrita
    if(my_rank != 0){
        f_saida = fopen(novo_nome_arquivo(argv[1]), "a");
    }

    //calculo paralelo (calcula e dá append no arquivo)
    calcula_vizinhos_comuns_paralelo(g, my_rank, num_clusters, f_saida);

    fclose(f_saida);
    free(g->matriz);
    free(g);

    MPI_Finalize();
    return 0;
}