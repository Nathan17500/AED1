#include <stdio.h>
#include <string.h>

#define MAX_NOME 30

typedef struct Contadores {
    long comparacoes;
    long trocas;
} Contadores;

typedef struct Jogador {
    int id;
    char nome[MAX_NOME];
    int pontuacao;
} Jogador;

int indiceMenor(int inicio, const int v[], int n, Contadores *c) {
    int menor = inicio;
    for (int i = inicio+1; i < n; i++) {
        c->comparacoes++;
        if (v[i] < v[menor]) {
            menor = i;
        }
    }
    return menor;
}

void selectionSort (int v[], int n, Contadores *c) {
    for (int i = 0; i < n-1; i++) {
        int menor = indiceMenor(i, v, n, c);
        if (menor != i) {
            int tmp = v[i];
            v[i] = v[menor];
            v[menor] = tmp;
            c->trocas++;
        }
    }
}

int posicionamento (const Jogador *a, const Jogador *b) {
    if (a->pontuacao != b->pontuacao) {
        return a->pontuacao > b->pontuacao;
    }

    int cmp = strcmp (a->nome, b->nome);
    if (cmp != 0) {
        return cmp < 0;
    }

    return a->id < b->id;
}

int indicePrimeiro (const Jogador v[], int n, int inicio, Contadores *c) {
    int melhor = inicio;
    for (int i = inicio + 1; i < n;i++) {
        c->comparacoes++;
        if (posicionamento(&v[i], &v[melhor])) {
            melhor = i;
        }
    }
    return melhor;
}

void selectionSortJogadores (Jogador v[], int n, Contadores *c) {
    for (int i = 0;i < n - 1; i++) {
        int melhor = indicePrimeiro(v, n, i, c);
        if (melhor != i) {
            Jogador tmp = v[i];
            v[i] = v[melhor];
            v[melhor] = tmp;
            c->trocas++;
        }
    }
}

int main () {
    const int pontuacoes_originais[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int n_num = sizeof(pontuacoes_originais) / sizeof(pontuacoes_originais[0]);

    const Jogador jogadores_originais[] = {
        {1, "Nathan", 80}, {2, "Joao", 95}, {3, "Caua", 80}, {4, "Rhaic", 70}, {5, "Lucas", 80}, {6, "Paulo", 95}
    };
    const int n_jog = sizeof(jogadores_originais) / sizeof(jogadores_originais[0]);

    int pontuacoes[8];
    memcpy(pontuacoes, pontuacoes_originais, n_num * sizeof(int));
    Jogador jogadores[6];
    memcpy(jogadores, jogadores_originais, n_jog * sizeof(Jogador));

    Contadores c_num = {0, 0};
    Contadores c_jog = {0, 0};

    selectionSort(pontuacoes, n_num, &c_num);
    selectionSortJogadores(jogadores, n_jog, &c_jog);

    printf("Numerico: comparacoes=%ld trocas=%ld\n", c_num.comparacoes, c_num.trocas);
    for (int i = 0; i < n_num; i++) printf("%d ", pontuacoes[i]);

    printf("\n\nRanking: comparacoes=%ld trocas=%ld\n", c_jog.comparacoes, c_jog.trocas);
    for (int i = 0; i < n_jog; i++) 
        printf("%d. %-6s id=%d pontos=%d\n", i + 1,
               jogadores[i].nome, jogadores[i].id, jogadores[i].pontuacao);

    return 0;
}