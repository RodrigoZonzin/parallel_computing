#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){

    int rank, nprocs; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    MPI_Datatype novo_tipo_subgrafo;
    
    /*int MPI_Type_vector(int count, int blocklength, int stride,
        MPI_Datatype oldtype, MPI_Datatype *newtype)
    */
    
    MPI_Type_vector(6, 2, 2, MPI_INT, &novo_tipo_subgrafo);
    MPI_Type_commit(&novo_tipo_subgrafo);

    //codigo mestre
    if(rank ==0){
        //int meus_numeros[][2] = {{2, 4}, {6, 8}, {10, 12}, {14, 16}, {18, 20}}; 
        int meus_numeros[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
        int meus_numeros_size = 6;

        /*int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype,
            int root, MPI_Comm comm)
        */
        MPI_Bcast(&meus_numeros_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        

        MPI_Send(&meus_numeros[0], 1, novo_tipo_subgrafo, 1, 1, MPI_COMM_WORLD);

    }

    //codigo obediente
    else if(rank > 0){
        int size_rcv; 
        
        MPI_Bcast(&size_rcv, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Valor recebido: %d\n", size_rcv); 

        int *dados_recebidos = (int*)malloc(sizeof(int)*size_rcv); 

        /*
        int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
            int source, int tag, MPI_Comm comm, MPI_Status *status)
        */
        printf("oii\n");
        MPI_Recv(dados_recebidos, 1, novo_tipo_subgrafo, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //for(int i =0; i<1; i++){
        //    printf("%d \n", dados_recebidos[i]);
        //}
        printf("%d\n", dados_recebidos[0]); 
    
    }


    MPI_Finalize();
    return 0; 
}