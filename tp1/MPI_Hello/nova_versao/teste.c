#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int tamanho;
    int* matriz;
} Grafo;

int determina_vizinhos(int* v1, int* v2, int n) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (v1[i] == v2[i] && (v1[i] != 0 || v2[i] != 0)) {
            k++;
        }
    }
    return k;
}

Grafo* faz_grafo(int n) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->tamanho = n;
    g->matriz = (int*)malloc(n * n * sizeof(int));
    memset(g->matriz, 0, n * n * sizeof(int));
    return g;
}

void insere_aresta(Grafo* g, int u, int v, int peso) {
    g->matriz[u * g->tamanho + v] = peso;
    g->matriz[v * g->tamanho + u] = peso;
}

int main(int argc, char** argv) {
    int my_rank, num_procs;
    Grafo* g = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

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


    int* local_matriz = NULL;
    int local_linhas;

    // Mestre inicializa o grafo e envia para os outros processos

    // Broadcast do tamanho do grafo para todos os processos
    MPI_Bcast(&grafo_tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Determina o número de linhas para cada processo
    local_linhas = grafo_tamanho / num_procs;
    int resto = grafo_tamanho % num_procs;

    // Ajusta linhas adicionais para os processos iniciais
    if (my_rank < resto) {
        local_linhas++;
    }

    // Aloca memória para o subgrafo de cada processo
    local_matriz = (int*)malloc(local_linhas * grafo_tamanho * sizeof(int));

    // Scatter para enviar partes da matriz de adjacência
    int* sendcounts = (int*)malloc(num_procs * sizeof(int));
    int* displs = (int*)malloc(num_procs * sizeof(int));

    if (my_rank == 0) {
        int offset = 0;
        for (int i = 0; i < num_procs; i++) {
            sendcounts[i] = (grafo_tamanho / num_procs + (i < resto ? 1 : 0)) * grafo_tamanho;
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    MPI_Scatterv(
        g ? g->matriz : NULL, sendcounts, displs, MPI_INT,
        local_matriz, local_linhas * grafo_tamanho, MPI_INT,
        0, MPI_COMM_WORLD);

    // Cada processo calcula os vizinhos em sua parte
    int local_result = 0;
    for (int i = 0; i < local_linhas - 1; i++) {
        for (int j = i + 1; j < local_linhas; j++) {
            local_result += determina_vizinhos(
                &local_matriz[i * grafo_tamanho], 
                &local_matriz[j * grafo_tamanho], 
                grafo_tamanho);
        }
    }

    // Reduce para somar todos os resultados no mestre
    int total_result = 0;
    MPI_Reduce(&local_result, &total_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("Total de vizinhos entre linhas: %d\n", total_result);
        free(g->matriz);
        free(g);
    }

    free(local_matriz);
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}
