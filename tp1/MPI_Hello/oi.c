#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define fimLinha printf("\n");

int main(int argc, char** argv){
    int process_rank, size_of_clusters, message_item;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_clusters);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    //printf("Size of clusters: %d\n", size_of_clusters);
    int buf_size = 15; 

    if(process_rank == 0){
        message_item = 42;
        
        int varios_dados[] = {2,5,8,9,6,3,1,4,7,1,2,3,9,8,7};

        //int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
        MPI_Send(&varios_dados, buf_size, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("Mensagem enviada no processo %d\n", process_rank);
    }

    
    else if(process_rank == 1){
        int *recebido = malloc(sizeof(MPI_INT)*buf_size);
        
        //int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status * status)
        MPI_Recv(recebido, buf_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i = buf_size; i>= 0; i--){
            printf("%d ", recebido[i]);
        }
        fimLinha
        printf("Fim dos dados recebidos");
        fimLinha
        free(recebido);
    }
    
    MPI_Finalize();
    return 0;
}