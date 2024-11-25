#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numero_elementos_por_processo;
    int *meus_numeros = NULL;
    int *recvbuffer;

    if (rank == 0) {
        int total_elementos = atoi(argv[1]);                   // Exemplo: total de elementos
        int *vetor = (int*)malloc(sizeof(int)*total_elementos);
        for(int i = 0; i<total_elementos; i++){
            scanf("%d", &vetor[i]); 
        }

        for(int i = 0; i<total_elementos; i++){
            printf("%d ", vetor[i]); 
        }

        numero_elementos_por_processo = total_elementos / nprocs;

        printf("Total de elementos: %d\n Elementos por processo: %d\n",
               total_elementos, numero_elementos_por_processo);
    }

    // Compartilhar o número de elementos por processo
    MPI_Bcast(&numero_elementos_por_processo, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Alocar o buffer de recebimento em cada processo
    recvbuffer = malloc(numero_elementos_por_processo * sizeof(int));

    // Dispersão dos dados entre os processos
    MPI_Scatter(meus_numeros, numero_elementos_por_processo, MPI_INT,
                recvbuffer, numero_elementos_por_processo, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo imprime os dados recebidos
    printf("Rank %d recebeu:", rank);
    for (int i = 0; i < numero_elementos_por_processo; i++) {
        printf(" %d", recvbuffer[i]);
    }
    printf("\n");

    // Liberar memória
    if (rank == 0) {
        free(meus_numeros);
    }
    free(recvbuffer);

    MPI_Finalize();
    return 0;
}
