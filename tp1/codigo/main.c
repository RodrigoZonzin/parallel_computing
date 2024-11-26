#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"
#include "Utilitarios.h"

int main(int argc, char **argv){   
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
    
    for(int i =0; i< g->tamanho; i++){
        int k, N_intersec; 
        int *intersec;
        
        for(int j = 0; j < g->tamanho; j++){
            //imprime só a matriz triangular inferior
            if(i >= j) continue;

            //determina_vizinhos() retorna um array de int com os vizinhos
            //k representa o numero de vizinhos entre os vertices i e j
            k = determina_vizinhos(g, i, j, &N_intersec, &k);
            
            //se o número de vizinhos for inferior a 1, pula
            if(k <= 0) continue;

            fprintf(f_saida, "%d %d %d\n", i, j, k);
        }
    }

    fclose(f_saida);
    fclose(f);
    return 0;
}
