#include <stdio.h>
#include <stdlib.h>

int main () {
    int linhas, colunas;

    printf("Informe o numero de linhas:");
    scanf("%d", &linhas);
    int ** matriz = malloc(linhas*sizeof(int*));
    printf("Informe o numero de colunas:");
    scanf("%d", &colunas);
    for (int i=0; i<linhas;i++) {
        matriz[i] = malloc(colunas*sizeof(int));
        if(matriz[i]==NULL){
            printf("Sem memoria.");
            exit(1);
        }
    }
    printf("Insira os elementos da matriz:");
    for (int p=0;p<)
    return 0;
}