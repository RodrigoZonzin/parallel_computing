#define _TUPLA_H

#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura Tupla
struct tupla {
    int u;
    int v;
    int k;
};
typedef struct tupla Tupla;

// Definição da estrutura Vetor de Tuplas
struct vetor_tupla {
    Tupla* vet; // Vetor dinâmico de Tuplas
    int tam;    // Número de tuplas no vetor
    int capacidade; // Capacidade atual do vetor
};
typedef struct vetor_tupla VetorTuplas;

// Função para criar um vetor de tuplas
VetorTuplas* criaVetorTuplas() {
    VetorTuplas* vetor = (VetorTuplas*)malloc(sizeof(VetorTuplas));
    vetor->vet = (Tupla*)malloc(10 * sizeof(Tupla)); // Inicialmente suporta 10 tuplas
    vetor->tam = 0;
    vetor->capacidade = 10;
    return vetor;
}

// Função para criar uma tupla
Tupla criaTupla(int u, int v, int k) {
    Tupla t;
    t.u = u;
    t.v = v;
    t.k = k;
    return t;
}

// Função para adicionar uma tupla ao vetor de tuplas
void append(VetorTuplas* vetor, int u, int v, int k) {
    // Verifica se há espaço suficiente, caso contrário, dobra a capacidade
    if (vetor->tam >= vetor->capacidade) {
        vetor->capacidade *= 2;
        vetor->vet = (Tupla*)realloc(vetor->vet, vetor->capacidade * sizeof(Tupla));
    }
    // Adiciona a nova tupla ao vetor
    vetor->vet[vetor->tam] = criaTupla(u, v, k);
    vetor->tam++;
}

// Função para imprimir o vetor de tuplas
void imprimeVetorTuplas(VetorTuplas* vetor) {
    for (int i = 0; i < vetor->tam; i++) {
        printf("Tupla %d: (%d, %d, %d)\n", i + 1, vetor->vet[i].u, vetor->vet[i].v, vetor->vet[i].k);
    }
}

// Função para liberar a memória do vetor de tuplas
void liberaVetorTuplas(VetorTuplas* vetor) {
    free(vetor->vet);
    free(vetor);
}

// Função para fazer o merge de dois VetorTuplas
VetorTuplas* merge(VetorTuplas* vet1, VetorTuplas* vet2) {
    // Cria um novo vetor para armazenar o resultado
    VetorTuplas* resultado = criaVetorTuplas();

    // Ajusta a capacidade inicial para evitar realocações desnecessárias
    resultado->capacidade = vet1->tam + vet2->tam;
    resultado->vet = (Tupla*)realloc(resultado->vet, resultado->capacidade * sizeof(Tupla));

    // Copia as tuplas do primeiro vetor para o resultado
    for (int i = 0; i < vet1->tam; i++) {
        resultado->vet[resultado->tam++] = vet1->vet[i];
    }

    // Copia as tuplas do segundo vetor para o resultado
    for (int i = 0; i < vet2->tam; i++) {
        resultado->vet[resultado->tam++] = vet2->vet[i];
    }

    return resultado;
}





