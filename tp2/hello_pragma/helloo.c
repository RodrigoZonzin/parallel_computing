#include <stdlib.h>
#include <stdio.h>
#include <omp.h>



int main(){

    printf("Num de threads eh: %d\n", omp_get_num_threads());

    printf("Num maximo de threads eh: %d\n",omp_get_max_threads());

    printf("Num maximo de processadores eh: %d\n", omp_get_num_procs());

    printf("Mostra numero do thread atual: %d\n", omp_get_thread_num());


    #pragma omp parallel num_threads(4)
    {
        printf("Hello from Thread: %d\n", omp_get_thread_num());
    }

    return 0;
}