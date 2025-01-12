#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int compare(const void* a, const void* b){
   return (*(int*)a - *(int*)b);
}

void selection_sort(int arr[], int n) {
    #pragma omp parallel for
    for (int i = 0; i < n - 1; i++) {
        // Find the index of the minimum element in the unsorted portion
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }

        // Swap the found minimum element with the first element of the unsorted portion
        if (min_index != i) {
            int temp = arr[i];
            arr[i] = arr[min_index];
            arr[min_index] = temp;
        }
    }
}

int main(int argc, char **argv){

    int n = atoi(argv[1]);

    int *vet = (int*)malloc(sizeof(int)*n);

    for(int i =0; i<n; i++){
        vet[i] = (int)rand();
    }

    //qsort(vet, n, sizeof(int), compare);
    printf("Numero de processadores: %d\n", omp_get_num_procs()); 
    selection_sort(vet, n);

    return 0; 
}