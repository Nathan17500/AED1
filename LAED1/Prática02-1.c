#include <stdio.h>
#include <stdlib.h>

int main () {
    int n;

    printf("Informe o tamanho do vetor:");
    scanf("%d", &n);

    int * vetor = (int*) malloc(n*sizeof(int));
    if(vetor==NULL){
        printf("Nao ha memoria.");
        exit(1);
    } else {
        printf("Alocacao realizada!\n");
    }
    printf("Informe os elementos do vetor:\n");
    for (int i=0;i<n;i++) {
        scanf("%d", &vetor[i]);
    }
    for (int p=n-1;p>=0;p--) {
        printf("%d\n", vetor[p]);
    }

    return 0;
}