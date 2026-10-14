#include <stdio.h>
#include <stdlib.h>

typedef struct Contadores {
    long acessos;
    long comparacoes;
} Contadores;

int acessoIndice(const int *v, int n, int indice, int *resultado, Contadores *c) {
    if (indice < 0 || indice >= n) {
        return 0;
    }
    *resultado = v[indice];
    c->acessos++;
    return 1; 
}

int maiorValor (const int *v, int n, Contadores *c) {
    int maior = v[0];
    c->acessos++;

    for (int i = 1; i < n; i++) {
        c->acessos++;
        c->comparacoes++;
        if (v[i] > maior) {
            maior = v[i];
        }
    }
    
    return maior;
}

int main () {
    int tamanhos[3] = {10, 100 , 1000};
    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("%8s | %16s | %16s | %20s\n", "n" , "direto(acessos)", "maior(acessos)", "maior(comparacoes)");
    
    for (int t = 0; t < quantidade; t++) {
        int n = tamanhos[t];
        int *vetor = malloc(n * sizeof(int));
        if (vetor == NULL) {
            printf("Falha na alocacao.\n");
            return 1;
        }
        for (int i = 0; i < n; i++) {
            vetor[i] = (i * 7) % 1000;
        }

        Contadores c_indice = {0, 0};
        Contadores c_maior = {0, 0};

        int valor;
        acessoIndice(vetor, n, n / 2, &valor, &c_indice);
        maiorValor(vetor, n, &c_maior);

        printf("%8d | %16ld | %16ld | %20ld\n", n, c_indice.acessos, c_maior.acessos, c_maior.comparacoes);

        free(vetor);
    }
    return 0;
}