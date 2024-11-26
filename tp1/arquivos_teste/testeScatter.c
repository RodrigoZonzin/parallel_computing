#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int VAL = 8/size;

    int sendbuf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int *recvbuf = (int*)malloc(sizeof(int)*VAL);  // Cada processo recebe 2 elementos

    // Distribuir os dados do processo raiz (rank 0)
    MPI_Scatter(sendbuf, VAL, MPI_INT, recvbuf, VAL, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processo %d recebeu: ", rank);
    for(int i =0; i<VAL;i++){
        printf("%d ", recvbuf[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}