#include <stdio.h>
#include <stdlib.h> 

void printa(int* vet, int n){
	for(int i=0; i<n;i++){
		printf("%d ", vet[i]); 
	}
	printf("\n"); 
}

int* slice_vetor(int *vet, int k, int m, int *n3){
    int *novo_vetor =(int*)malloc(sizeof(int)*m); 

    int j = 0; 
    for(int i=k;i<m; i++){
        novo_vetor[j] = vet[i]; 
        j++; 
    }

    *n3 = j; 
    return novo_vetor;
}


int main(){

	int vet[] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; 

	int *subvet1 = (int*)malloc(sizeof(int)*6); 
	int *subvet2 = (int*)malloc(sizeof(int)*3); 

	for(int i=0; i<6;i++){
		subvet1[i] = vet[i]; 
	}

	for(int i=0;i<3;i++){
		subvet2[i] = vet[i];
	}

	int n3; 
	int *subvet3 = slice_vetor(vet, 1, 4, &n3); 

	printa(vet, 9);
	printa(subvet1, 6);
	printa(subvet2, 3);
	printa(subvet3, n3);
	

	return 0; 
}
