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

bool posicionamentoPorNome (const Jogador *a, const Jogador *b) {
    int comparacao = strcmp (a->nome, b->nome);
    if (comparacao != 0) {
        return comparacao < 0;
    }
    return a->id < b->id;
}

bool estaOrdenadoPorNome (const Jogador v[], int n) {
    for (int i = 1; i < n; i++) {
        if (posicionamentoPorNome(&v[i], &v[i-1])) return false;
    }
    return true;
}

void insertionSortPorNome (Jogador v[], int n, Contadores_Insertion *c) {
    for (int i=1; i < n; i++) {
        Jogador chave = v[i];
        int j = i - 1;

        while (j>=0) {
            c->comparacoes++;
            if (posicionamentoPorNome(&chave, &v[j])) {
                v[j+1] = v[j];
                c->deslocamentos++;
                j--;
            } else {
                break;
            }
        }
        v[j+1] = chave;
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

bool registrosPreservados (const Jogador origem[], const Jogador ordenado[], int n) {
    for (int i = 0; i < n; i++) {
        bool achou = false;
        for (int k = 0; k < n; k++) {
            if (ordenado[k].id == origem[i].id) {
                if (strcmp(ordenado[k].nome, origem[i].nome) != 0 || ordenado[k].pontuacao != origem[i].pontuacao) {
                    return false;
                }
                achou = true;
                break;
            }
        }
        if (!achou) return false;
    }
    return true;
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

void exibirJogadores (const char *titulo, const Jogador v[], int n) {
    printf("\n%s\n", titulo);
    for (int i = 0; i < n;i++) {
        printf("%d. %-6s id=%d pontos=%d\n", i+1, v[i].nome, v[i].id, v[i].pontuacao);
    }
}

void Numerica(void) {
    const int pontuacoes_originais[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int n_num = sizeof(pontuacoes_originais) / sizeof(pontuacoes_originais[0]);

    int pontuacoes_selection[8];
    int pontuacoes_insertion[8];
    memcpy(pontuacoes_selection, pontuacoes_originais, n_num * sizeof(int));
    memcpy(pontuacoes_insertion, pontuacoes_originais, n_num * sizeof(int));

    Contadores_Selection c_nums = {0, 0};
    Contadores_Insertion c_numi = {0, 0};

    selectionSort(pontuacoes_selection, n_num, &c_nums);
    insertionSort(pontuacoes_insertion, n_num, &c_numi);

    printf("\nSelection: comparacoes=%ld trocas=%ld\n", c_nums.comparacoes, c_nums.trocas);
    for (int i = 0; i < n_num; i++) printf("%d ", pontuacoes_selection[i]);

    printf("\n\nInsertion: comparacoes=%ld deslocamentos=%ld\n", c_numi.comparacoes, c_numi.deslocamentos);
    for (int i = 0; i < n_num; i++) printf("%d ", pontuacoes_insertion[i]);

    printf("\n\nInsertion ordenado? %s\n", estaOrdenado(pontuacoes_insertion, n_num) ? "sim" : "nao");
}

void Ranking(void) {
    const Jogador jogadores_originais[] = {
        {1, "Nathan", 80}, {2, "Joao", 95}, {3, "Caua", 80},
        {4, "Rhaic", 70}, {5, "Lucas", 80}, {6, "Paulo", 95}
    };
    const int n_jog = sizeof(jogadores_originais) / sizeof(jogadores_originais[0]);

    Jogador jogadores[6];
    memcpy(jogadores, jogadores_originais, n_jog * sizeof(Jogador));

    Contadores_Selection c_jog = {0, 0};
    selectionSortJogadores(jogadores, n_jog, &c_jog);

    printf("\nRanking: comparacoes=%ld trocas=%ld\n", c_jog.comparacoes, c_jog.trocas);
    for (int i = 0; i < n_jog; i++)
        printf("%d. %-6s id=%d pontos=%d\n", i + 1,
               jogadores[i].nome, jogadores[i].id, jogadores[i].pontuacao);
}

void CatalogoPorNome (void) {
    const Jogador catalogo_original[] = { {7, "Joao",   60}, {1, "Nathan", 80}, {2, "Joao",  95}, {3, "Caua",  80}, {4, "Rhaic",  70}, {5, "Lucas",  80}, {6, "Paulo", 95}, {8, "Caua",  90} };
    const int n = sizeof(catalogo_original) / sizeof(catalogo_original[0]);

    Jogador catalogo[8];
    memcpy(catalogo, catalogo_original, n * sizeof(Jogador));

    Contadores_Insertion c = {0, 0};

    exibirJogadores("Antes (ordem de cadastro):", catalogo, n);
    insertionSortPorNome(catalogo, n, &c);
    exibirJogadores("Depois (ordem alfabetica):", catalogo, n);

    printf("\nInsertion: comparacoes=%ld deslocamentos=%ld\n", c.comparacoes, c.deslocamentos);
    printf("Ordenado por nome? %s\n", estaOrdenadoPorNome(catalogo, n) ? "sim" : "nao");
    printf("Campos preservados? %s\n", registrosPreservados(catalogo_original, catalogo, n) ? "sim" : "nao");
}

int main () {
    int opcao;

    do {
        printf("\n----MENU----\n");
        printf("1.Ordenacao numerica (Selection X Insertion).\n2.Ranking de Jogadores (Selection).\n3.Catalogo por nome (Insertion).\n0.Sair.\nEscolha: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1: Numerica();         break;
            case 2: Ranking();          break;
            case 3: CatalogoPorNome();  break;
            case 0:                     break;
            default: printf("Opcao Invalida.\n");
        }
    } while (opcao != 0);
    return 0;
}