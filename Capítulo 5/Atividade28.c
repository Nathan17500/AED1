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

int encontrarPares (const int *v, int n, int alvo, Contadores *c) {
    int encontrados = 0;

    for (int i = 0; i < n-1; i++) {
        for (int j = i + 1; j < n; j++) {
            c->comparacoes++;
            if (v[i] + v[j] == alvo) {
                encontrados++;
            }
        }
    }
    return encontrados;
}

int buscaBinaria(const int *v, int n, int alvo, Contadores *c, int mostrar) {
    int inicio = 0;
    int fim = n-1;
    int passo = 0;

    while (inicio <= fim) {
            int meio = inicio + (fim - inicio)/2;
            passo++;
            c->comparacoes++;

            if (mostrar) {
                printf("passo %d: inicio=%d meio=%d fim=%d (tamanho=%d) v[meio]=%d\n", passo, inicio, meio, fim, fim - inicio + 1, v[meio]);
            }

            if (v[meio] == alvo) {
                return meio;
            } else if (v[meio] < alvo) {
                inicio = meio + 1;
            } else {
                fim = meio - 1;
            }
    }
    return -1;
}

int estarOrdenado (const int *v, int n) {
    for (int i = 1; i < n;i++) {
        if (v[i-1] > v[i]) {
            return 0;
        }
    }
    return 1;
}

int main () {
    int resposta;

    printf("Escolha qual demonstracao sera mostrada:\n1.O(1),O(n) e O(n^2)\n2.O(log n)\n");
    scanf("%d", &resposta);
    while (resposta != 1 && resposta != 2) {
        printf("Valor inválido. Informe novamente.\n");
        scanf("%d", &resposta);
    }
    if (resposta == 1) {
        int tamanhos[3] = {10, 100 , 1000};
        int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

        printf("%8s | %16s | %16s | %20s | %20s\n", "n" , "direto(acessos)", "maior(acessos)", "maior(comparacoes)", "pares(comparacoes)");
        
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
            Contadores c_ausente = {0, 0};
            Contadores c_pares = {0, 0};

            int valor;
            acessoIndice(vetor, n, n / 2, &valor, &c_indice);
            maiorValor(vetor, n, &c_ausente);
            encontrarPares(vetor, n, 100, &c_pares);

            printf("%8d | %16ld | %16ld | %20ld | %20ld\n", n, c_indice.acessos, c_ausente.acessos, c_ausente.comparacoes, c_pares.comparacoes);

            free(vetor);
        }
    } else {
        int tamanhos[3] = {10, 100, 1000};
        int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

        printf ("%8s | %13s | %15s | %12s | %15s | %14s\n", "n", "pos(presente)", "comp(presente)", "pos(ausente)", "comp(ausente)", "piso(log2n)+1");

        for (int t = 0; t < quantidade; t++) {
            int n = tamanhos[t];
            int *vetor = malloc(n * sizeof(int));
            if (vetor == NULL) {
                printf("Falha na alocacao.\n");
                return 1;
            }
            for (int i = 0; i < n; i++) {
                vetor[i] = 2 * i;
            }

            if (!estarOrdenado(vetor, n)) {
                printf("Vetor nao ordenado.\n");
                free(vetor);
                return 1;
            }

            Contadores c_ultimo = {0, 0};
            Contadores c_ausente = {0, 0};

            int pos_ultimo = buscaBinaria(vetor, n, 2 * (n-1), &c_ultimo, 0);
            int pos_ausente = buscaBinaria(vetor, n, 2 * n, &c_ausente, 0);

            int teorico = 0;
            for (int k = n; k > 0; k /= 2) {
                teorico++;
            }

            printf("%8d | %13d | %15ld | %12d | %15ld | %14d\n", n, pos_ultimo, c_ultimo.comparacoes, pos_ausente, c_ausente.comparacoes, teorico);
            free(vetor);
        }

        printf("\nPassos da busca por 29 em v[i] = 2*i, n = 16:\n");
        int pequeno[16];
        for (int i = 0; i < 16; i++) {
            pequeno[i] = 2 * i;
        }
        Contadores c_demo = {0, 0};
        int pos = buscaBinaria(pequeno, 16, 29, &c_demo, 1);
        printf("posicao = %d, comparacoes = %ld\n", pos, c_demo.comparacoes);
    }
    return 0;
}