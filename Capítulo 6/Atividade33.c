#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 30

typedef struct Contadores_Insertion {
    long comparacoes;
    long deslocamentos;
} Contadores_Insertion;

typedef struct Contadores_Selection {
    long comparacoes;
    long trocas;
} Contadores_Selection;

typedef struct Jogador {
    int id;
    char nome[MAX_NOME];
    int pontuacao;
} Jogador;

bool estaOrdenado (const int v[], int n) {
    for (int i = 1; i < n; i++) {
        if (v[i-1] > v[i]) return false;
    }
    return true;
}

int indiceMenor(int inicio, const int v[], int n, Contadores_Selection *c) {
    int menor = inicio;
    for (int i = inicio+1; i < n; i++) {
        c->comparacoes++;
        if (v[i] < v[menor]) {
            menor = i;
        }
    }
    return menor;
}

void insercaoOrdenada (int v[], int i, Contadores_Insertion *c) {
    int chave = v[i];
    int j = i -1;

    while (j >= 0) {
        c->comparacoes++;
        if (v[j] > chave) {
            v[j+1] = v[j];
            c->deslocamentos++;
            j--;
        } else {
            break;
        }
    }
    v[j+1] = chave;
}

void insertionSort(int v[], int n, Contadores_Insertion *c) {
    for (int i = 1; i < n; i++) {
        insercaoOrdenada(v, i, c);
    }
}

void selectionSort (int v[], int n, Contadores_Selection *c) {
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

int indicePrimeiro (const Jogador v[], int n, int inicio, Contadores_Selection *c) {
    int melhor = inicio;
    for (int i = inicio + 1; i < n;i++) {
        c->comparacoes++;
        if (posicionamento(&v[i], &v[melhor])) {
            melhor = i;
        }
    }
    return melhor;
}

void selectionSortJogadores (Jogador v[], int n, Contadores_Selection *c) {
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
    int pontuacoes_insertion[8];
    memcpy(pontuacoes_insertion, pontuacoes_originais, n_num * sizeof(int));
    Jogador jogadores[6];
    memcpy(jogadores, jogadores_originais, n_jog * sizeof(Jogador));

    Contadores_Selection c_nums = {0, 0};
    Contadores_Selection c_jog = {0, 0};
    Contadores_Insertion c_numi = {0, 0};

    selectionSort(pontuacoes, n_num, &c_nums);
    selectionSortJogadores(jogadores, n_jog, &c_jog);

    insertionSort(pontuacoes_insertion, n_num, &c_numi);

    printf("Selection: comparacoes=%ld trocas=%ld\n", c_nums.comparacoes, c_nums.trocas);
    for (int i = 0; i < n_num; i++) printf("%d ", pontuacoes[i]);

    printf("\n\nInsertion: comparacoes=%ld deslocamentos=%ld\n", c_numi.comparacoes, c_numi.deslocamentos);
    for (int i = 0; i < n_num; i++) printf("%d ", pontuacoes_insertion[i]);

    printf("\n\nInsertion ordenado? %s\n", estaOrdenado(pontuacoes_insertion, n_num) ? "sim" : "nao");

    printf("\nRanking: comparacoes=%ld trocas=%ld\n", c_jog.comparacoes, c_jog.trocas);
    for (int i = 0; i < n_jog; i++) 
        printf("%d. %-6s id=%d pontos=%d\n", i + 1,
               jogadores[i].nome, jogadores[i].id, jogadores[i].pontuacao);

    return 0;
}