#include <stdio.h>
#include <stdlib.h>

#define FIB_MAX 35

typedef struct Contadores {
    long acessos;
    long comparacoes;
} Contadores;

typedef struct Recursao {
    long chamadas;
    int profundidade_atual;
    int profundidade_max;
} Recursao;

long fibonacciRec(int n, Recursao *r, int mostrar) {
    r->chamadas++;
    r->profundidade_atual++;
    if (r->profundidade_atual > r->profundidade_max) {
        r->profundidade_max = r->profundidade_atual;
    }

    if (mostrar) {
        printf("%*schamada: n=%d\n", (r->profundidade_atual - 1) * 2, "", n);
    }

    long resultado;
    const char *motivo;
    if (n <= 1) {
        resultado = n;
        motivo = "caso-base";
    } else {
        long a = fibonacciRec(n - 1, r, mostrar);
        long b = fibonacciRec(n - 2, r, mostrar);
        resultado = a + b;
        motivo = "soma de duas chamadas";
    }

    if (mostrar) {
        printf("%*svolta: n=%d -> %ld [%s]\n", (r->profundidade_atual - 1) * 2, "", n, resultado, motivo);
    }

    r->profundidade_atual--;
    return resultado;
}

int fibonacciSeguro(int n, Recursao *r, long *resultado) {
    if (n < 0 || n > FIB_MAX) {
        return 0;
    }
    *resultado = fibonacciRec(n, r, 0);
    return 1;
}

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

int buscaLinear(const int *v, int n, int alvo, Contadores *c, int mostrar) {
    for (int i = 0; i < n; i++) {
        c->comparacoes++;

        if (mostrar) {
            printf("passo %d: indice=%d v[i]=%d\n", i+1, i, v[i]);
        }

        if (v[i] == alvo) {
            return i;
        }
    }

    return -1;
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

int buscaBinariaRec(const int *v, int inicio, int fim, int alvo, Recursao *r, int mostrar) {
    r->chamadas++;
    r->profundidade_atual++;
    if (r->profundidade_atual > r->profundidade_max) {
        r->profundidade_max = r->profundidade_atual;
    }

    if (mostrar) {
        printf("%*schamada: inicio=%d fim=%d\n", (r->profundidade_atual - 1) * 2, "", inicio, fim);
    }

    int posicao;
    const char *motivo;
    if (inicio > fim) {
        posicao = -1;
        motivo = "intervalo vazio (caso-base)";
    } else {
        int meio = inicio + (fim - inicio) / 2;
        if (v[meio] == alvo) {
            posicao = meio;
            motivo = "achou (caso-base)";
        } else if (v[meio] < alvo) {
            posicao = buscaBinariaRec(v, meio + 1, fim, alvo, r, mostrar);
            motivo = "resultado da chamada menor";
        } else {
            posicao = buscaBinariaRec(v, inicio, meio - 1, alvo, r, mostrar);
            motivo = "resultado da chamada menor";
        }
    }

    if (mostrar) {
        printf("%*svolta: inicio=%d fim=%d -> %d [%s]\n",
               (r->profundidade_atual - 1) * 2, "", inicio, fim, posicao, motivo);
    }

    r->profundidade_atual--;
    return posicao;
}

int estarOrdenado (const int *v, int n) {
    for (int i = 1; i < n;i++) {
        if (v[i-1] > v[i]) {
            return 0;
        }
    }
    return 1;
}

long somaAteRecursao (int n, Recursao *r, int mostrar) {
    r->chamadas++;
    r->profundidade_atual++;
    if (r->profundidade_atual > r->profundidade_max) {
        r->profundidade_max = r->profundidade_atual;
    }

    if (mostrar) {
        printf("%*sdesce: n=%d\n", (r->profundidade_atual - 1) * 2, "", n);
    }

    long resultado;
    if (n <= 0) {
        resultado = 0;
    } else {
        resultado = n + somaAteRecursao(n-1, r, mostrar);
    }

    if (mostrar) {
        printf("%*svolta: n=%d -> %ld\n", (r->profundidade_atual -1) * 2, "", n, resultado);
    }

    r->profundidade_atual--;
    return resultado;
}

long somaAteIteracao (int n, long *iteracoes) {
    long soma = 0;
    for (int i = 1; i <= n; i++) {
        (*iteracoes)++;
        soma += i;
    }
    return soma;
}

long somaVetorRecursao(const int *v, int n, Recursao *r, int mostrar) {
    r->chamadas++;
    r->profundidade_atual++;
    if (r->profundidade_atual > r->profundidade_max) {
        r->profundidade_max = r->profundidade_atual;
    }

    if (mostrar) {
        printf("%*sdesce: n=%d\n", (r->profundidade_atual - 1) * 2, "", n);
    }

    long resultado;
    if (n <= 0) {                                              // caso-base: vetor vazio
        resultado = 0;
    } else {
        resultado = v[n - 1] + somaVetorRecursao(v, n - 1, r, mostrar);
    }

    if (mostrar) {
        printf("%*svolta: n=%d -> %ld\n", (r->profundidade_atual - 1) * 2, "", n, resultado);
    }

    r->profundidade_atual--;
    return resultado;
}

long somaVetorIteracao(const int *v, int n, long *iteracoes) {
    long soma = 0;
    for (int i = 0; i < n; i++) {
        (*iteracoes)++;
        soma += v[i];
    }
    return soma;
}

int main () {
    int resposta;
    do {
        printf("Escolha qual demonstracao sera mostrada:\n1.O(1),O(n) e O(n^2)\n2.O(log n)\n3.Busca linear x binaria\n4.Recursao\n5.Recursao ramificada x busca binaria recursiva\n0.Sair\n");
        scanf("%d", &resposta);
        while (resposta < 0 || resposta > 5) {
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
        } else if (resposta == 2){
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
        } else if (resposta == 3) {
            int tamanhos[3] = {10, 100, 1000};
            int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);
            const char *nomes[4] = {"inicio", "meio", "fim", "ausente"};

            printf("Busca linear x Busca binaria (vetor ordenado; v[i] = 2 * i)\n");
            printf("%8s | %-8s | %11s | %12s | %12s | %13s\n", "n", "alvo", "linear(pos)", "linear(comp)", "binaria(pos)", "binaria(comp)");

            for (int t = 0; t < quantidade; t++) {
                int n = tamanhos[t];
                int *vetor = malloc(n * sizeof(int));
                if (vetor == NULL) {
                    printf("Falha na alocacao.\n");
                    return  1;
                } 
                for (int i = 0; i < n; i++) {
                    vetor[i] = 2 * i;
                }

                if (!estarOrdenado(vetor, n)) {
                    printf("Vetor nao ordenado.\n");
                    free(vetor);
                    return 1;
                }

                int alvos[4] = {0, 2 * ((n-1)/2), 2 * (n-1), 2 * n};

                for (int a = 0; a < 4; a++) {
                    Contadores c_linear = {0, 0};
                    Contadores c_binaria = {0, 0};

                    int pos_linear = buscaLinear (vetor, n, alvos[a], &c_linear, 0);
                    int pos_binaria = buscaBinaria (vetor, n, alvos[a], &c_binaria, 0);

                    printf("%8d | %-8s | %11d | %12ld | %12d | %13ld\n", n, nomes[a], pos_linear, c_linear.comparacoes, pos_binaria, c_binaria.comparacoes);
                }
                free(vetor);
            }

            printf ("\nVetor desordenado: {9, 3, 14, 7, 1, 12, 3, 8}\n");
            int desordenado[8] = {9, 3, 14, 7, 1, 12, 3, 8};
            int nd = 8;

            if (!estarOrdenado(desordenado, nd)) {
                printf("Vetor nao ordenado, nao e possivel usar busca binaria. Usando busca linear.\n");
            }

            int buscados[5] = {9, 1, 3, 8, 100};
            printf("%6s | %6s | %11s\n", "alvo", "pos", "comparacoes");
            for (int k = 0; k < 5; k++) {
                Contadores c_demo = {0, 0};
                int pos = buscaLinear(desordenado, nd, buscados[k], &c_demo, 0);
                printf("%6d | %6d | %11ld\n", buscados[k], pos, c_demo.comparacoes);
            }
        } else if (resposta == 4){
            int tamanhos[3] = {10, 100, 1000};
            int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

            printf("Soma de 1 ate 4, passo a passo:\n");
            Recursao r_num_demo = {0, 0, 0};
            long res_num_demo = somaAteRecursao(4, &r_num_demo, 1);
            printf("resultado = %ld | chamadas = %ld | profundidade maxima = %d\n\n",
                res_num_demo, r_num_demo.chamadas, r_num_demo.profundidade_max);

            printf("Soma do vetor {5, 2, 8, 1}, passo a passo:\n");
            int pequeno[4] = {5, 2, 8, 1};
            Recursao r_vet_demo = {0, 0, 0};
            long res_vet_demo = somaVetorRecursao(pequeno, 4, &r_vet_demo, 1);
            printf("resultado = %ld | chamadas = %ld | profundidade maxima = %d\n\n",
                res_vet_demo, r_vet_demo.chamadas, r_vet_demo.profundidade_max);

            printf("%8s | %-10s | %10s | %10s | %9s | %8s | %12s | %-7s\n",
                "n", "problema", "iterativa", "recursiva", "iteracoes", "chamadas", "profundidade", "confere");

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

                long it_num = 0;
                long iter_num = somaAteIteracao(n, &it_num);
                Recursao r_num = {0, 0, 0};
                long rec_num = somaAteRecursao(n, &r_num, 0);

                printf("%8d | %-10s | %10ld | %10ld | %9ld | %8ld | %12d | %-7s\n",
                    n, "soma 1..n", iter_num, rec_num, it_num,
                    r_num.chamadas, r_num.profundidade_max,
                    iter_num == rec_num ? "sim" : "nao");

                long it_vet = 0;
                long iter_vet = somaVetorIteracao(vetor, n, &it_vet);
                Recursao r_vet = {0, 0, 0};
                long rec_vet = somaVetorRecursao(vetor, n, &r_vet, 0);

                printf("%8d | %-10s | %10ld | %10ld | %9ld | %8ld | %12d | %-7s\n",
                    n, "soma vetor", iter_vet, rec_vet, it_vet,
                    r_vet.chamadas, r_vet.profundidade_max,
                    iter_vet == rec_vet ? "sim" : "nao");

                free(vetor);
            }
        } else if (resposta == 5) {
            printf("Fibonacci ingenuo: fib(4), passo a passo\n");
            Recursao r_fib_demo = {0, 0, 0};
            long fib_demo = fibonacciRec(4, &r_fib_demo, 1);
            printf("resultado = %ld | chamadas = %ld | profundidade maxima = %d\n\n",
                fib_demo, r_fib_demo.chamadas, r_fib_demo.profundidade_max);

            printf("%8s | %8s | %10s | %12s\n", "n", "fib(n)", "chamadas", "profundidade");
            int entradas[6] = {5, 10, 15, 20, 25, 30};
            for (int k = 0; k < 6; k++) {
                Recursao r_fib = {0, 0, 0};
                long fib;
                if (!fibonacciSeguro(entradas[k], &r_fib, &fib)) {
                    printf("n=%d recusado (limite %d)\n", entradas[k], FIB_MAX);
                    continue;
                }
                printf("%8d | %8ld | %10ld | %12d\n", entradas[k], fib, r_fib.chamadas, r_fib.profundidade_max);
            }

            Recursao r_recusa = {0, 0, 0};
            long fib_recusa;
            if (!fibonacciSeguro(50, &r_recusa, &fib_recusa)) {
                printf("\nfibonacci(50) recusado: o limite e %d (50 geraria mais de 40 bilhoes de chamadas)\n", FIB_MAX);
            }

            int pequeno[16];
            for (int i = 0; i < 16; i++) {
                pequeno[i] = 2 * i;
            }

            printf("\nBusca binaria recursiva por 22 em v[i] = 2*i, n = 16:\n");
            Recursao r_bin_a = {0, 0, 0};
            int pos_a = buscaBinariaRec(pequeno, 0, 15, 22, &r_bin_a, 1);
            printf("posicao = %d | chamadas = %ld | profundidade maxima = %d\n",
                pos_a, r_bin_a.chamadas, r_bin_a.profundidade_max);

            printf("\nBusca binaria recursiva por 29 em v[i] = 2*i, n = 16:\n");
            Recursao r_bin_b = {0, 0, 0};
            int pos_b = buscaBinariaRec(pequeno, 0, 15, 29, &r_bin_b, 1);
            printf("posicao = %d | chamadas = %ld | profundidade maxima = %d\n",
                pos_b, r_bin_b.chamadas, r_bin_b.profundidade_max);

            int tamanhos[3] = {10, 100, 1000};
            int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);
            const char *nomes[4] = {"inicio", "meio", "fim", "ausente"};

            printf("\n%8s | %-8s | %5s | %13s | %12s | %15s | %-7s\n",
                "n", "alvo", "pos", "chamadas(rec)", "profundidade", "comp(iterativa)", "confere");

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

                int alvos[4] = {0, 2 * ((n - 1) / 2), 2 * (n - 1), 2 * n};

                for (int a = 0; a < 4; a++) {
                    Recursao r_rec = {0, 0, 0};
                    Contadores c_iter = {0, 0};

                    int pos_rec  = buscaBinariaRec(vetor, 0, n - 1, alvos[a], &r_rec, 0);
                    int pos_iter = buscaBinaria(vetor, n, alvos[a], &c_iter, 0);

                    printf("%8d | %-8s | %5d | %13ld | %12d | %15ld | %-7s\n",
                        n, nomes[a], pos_rec, r_rec.chamadas, r_rec.profundidade_max,
                        c_iter.comparacoes, pos_rec == pos_iter ? "sim" : "nao");
                }
                free(vetor);
            }
        }
    } while (resposta != 0);
    return 0;
}