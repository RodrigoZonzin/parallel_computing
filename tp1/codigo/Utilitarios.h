#define UTILITARIOS_H


void printa_vetor(int* vet, int tam){
    for(int i=0; i<tam; i++){
        printf("%d ", vet[i]);
    }
    printf("\n");
}


int num_diferentes(int* vet, int tam){
    int num = 0;
    
    int encontrado[tam];
    for (int i = 0; i < tam; i++){
        encontrado[i] = 0; 
    }
    
    // Verifica elementos únicos
    for(int i = 0; i < tam; i++){
        int repetido = 0;
        for(int j = 0; j < num; j++){
            if(vet[i] == encontrado[j]){
                repetido = 1;
                break;
            }
        }
        if(!repetido){
            encontrado[num] = vet[i];
            num++;
        }
    }
    
    return num;
}


char* novo_nome_arquivo(char* nome_arquivo){
    char* ponto = strrchr(nome_arquivo, '.');
    

    // Criar uma cópia do nome do arquivo até o ponto (sem a extensão)
    size_t tam = ponto - nome_arquivo;
    char* novo_nome = (char*)malloc(tam + strlen(".cng") + 1); 
            
    //copia nome do arquivo sem extensao
    strncpy(novo_nome, nome_arquivo, tam);
    novo_nome[tam] = '\0';  
    
    // Adicionar a nova extensão ".cng"
    strcat(novo_nome, ".cng");
    
    return novo_nome;
}


int* determina_vizinhos(Grafo *g, int u, int v, int *return_size, int *return_k){
    int nu, nv;

    int *adj_u = obtem_lista_vertices_adj(g, u, &nu), 
        *adj_v = obtem_lista_vertices_adj(g, v, &nv);  


    // |UV| <= max(|U|, |V|)  
    //a cardinalidade do conjunto intersecção é, no máximo, igual a maior dos outros dois conjuntos
    int maxNuNv = (nu > nv) ? nu : nv;
    int *intersec = (int*)malloc(sizeof(int)*maxNuNv);
    for(int i =0; i<maxNuNv; i++){
        intersec[i] = -1;
    }

    //determina os vizinhos
    int k = 0;
    for(int i = 0; i < nu; i++){
        for(int j = 0; j< nv; j++){
            
            if(adj_u[i] == adj_v[j]){
                intersec[k] = adj_u[i];
                k++;
                break;
            }
        }
    }
    
    //determina o número de vizinhos
    k = 0;
    for(int i = 0; i< maxNuNv; i++){
        if(intersec[i] >= 0) k++;
    }

    free(adj_u);
    free(adj_v);
    *return_size = maxNuNv;
    *return_k = k; 
    return intersec;
}
