#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 30

typedef struct Contadores_Merge
{
    long comparacoes;
} Contadores_Merge;

typedef struct Contadores_Bubble
{
    long comparacoes;
    long trocas;
    long passagens;
} Contadores_Bubble;

typedef struct Contadores_Insertion 
{
    long comparacoes;
    long deslocamentos;
} Contadores_Insertion;

typedef struct Contadores_Selection 
{
    long comparacoes;
    long trocas;
} Contadores_Selection;

typedef struct Jogador 
{
    int id;
    char nome[MAX_NOME];
    int pontuacao;
} Jogador;

typedef enum {
    CAMPO_ID,
    CAMPO_PONTUACAO,
    CAMPO_NOME 
} Campo;

typedef enum {
    CRESCENTE,
    DECRESCENTE
} Direcao;

typedef struct Criterio
{
    Campo campo;
    Direcao direcao;
} Criterio;

const char *nomeCampo (Campo campo) {
    switch (campo) {
        case CAMPO_ID:        return "ID";
        case CAMPO_PONTUACAO: return "Pontuacao";
        case CAMPO_NOME:      return "Nome";
    }
    return "?";
}

const char *nomeDirecao (Direcao direcao) {
    return direcao == CRESCENTE ? "crescente" : "decrescente";
}

int compararJogadores (const Jogador *a, const Jogador *b, Criterio crit) {
    int r = 0;
    switch (crit.campo) {
        case CAMPO_ID:        r = (a->id > b->id) - (a->id < b->id); break;
        case CAMPO_PONTUACAO: r = (a->pontuacao > b->pontuacao) - (a->pontuacao < b->pontuacao); break;
        case CAMPO_NOME:      r = strcmp(a->nome, b->nome); break;
    }
    return crit.direcao == DECRESCENTE ? -r : r;
}

bool estaOrdenadoJogadores (const Jogador v[], int n, Criterio crit) {
    for (int i = 1; i < n; i++) {
        if (compararJogadores(&v[i-1], &v[i], crit) > 0) return false;
    }
    return true;
}

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

void bubbleSort (int v[], int n, Contadores_Bubble *c) {
    for (int p = 0; p < n - 1; p++) {
        bool trocou = false;
        c->passagens++;

        for (int j = 0; j < n - 1 - p; j++) {
            c->comparacoes++;
            if (v[j] > v[j+1]) {
                int tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
                c->trocas++;
                trocou = true;
            }
        }

        if (!trocou) break;
    }
}

void bubbleSortJogadores (Jogador v[], int n, Criterio crit, Contadores_Bubble *c) {
    for (int p = 0; p < n - 1; p++) {
        bool trocou = false;
        c->passagens++;

        for (int j = 0; j < n - 1 - p; j++) {
            c->comparacoes++;
            if (compararJogadores(&v[j], &v[j+1], crit) > 0) {
                Jogador tmp = v[j];     // copia a struct inteira:
                v[j] = v[j+1];          // id, nome e pontuacao
                v[j+1] = tmp;
                c->trocas++;
                trocou = true;
            }
        }

        if (!trocou) break;
    }
}

void executarBubbleJogadores (const Jogador original[], int n, Criterio crit) {
    Jogador v[16];
    memcpy(v, original, n * sizeof(Jogador));

    printf("\nCriterio: %s (%s)\n", nomeCampo(crit.campo), nomeDirecao(crit.direcao));
    exibirJogadores("Antes:", v, n);

    Contadores_Bubble c = {0, 0, 0};
    bubbleSortJogadores(v, n, crit, &c);

    exibirJogadores("Depois:", v, n);
    printf("\nComparacoes=%ld  Trocas=%ld  Passagens=%ld\n", c.comparacoes, c.trocas, c.passagens);
    printf("Ordem valida? %s\n", estaOrdenadoJogadores(v, n, crit) ? "sim" : "nao");
    printf("Campos preservados? %s\n", registrosPreservados(original, v, n) ? "sim" : "nao");

    // Parada antecipada: ordenar de novo um vetor que já está ordenado
    Contadores_Bubble c2 = {0, 0, 0};
    bubbleSortJogadores(v, n, crit, &c2);
    printf("\nSegunda execucao (ja ordenado): comparacoes=%ld trocas=%ld passagens=%ld\n",
           c2.comparacoes, c2.trocas, c2.passagens);
}

void executarBubble (const char *titulo, const int original[], int n) {
    int v[16];
    memcpy(v, original, n * sizeof(int));

    Contadores_Bubble c = {0, 0, 0};
    bubbleSort(v, n, &c);

    printf("\n== %s ==\n", titulo);
    printf("Antes : ");
    for (int i = 0; i < n; i++) printf("%d ", original[i]);
    printf("\nDepois: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\nComparacoes=%ld  Trocas=%ld  Passagens=%ld\n",
           c.comparacoes, c.trocas, c.passagens);
    printf("Ordenado? %s\n", estaOrdenado(v, n) ? "sim" : "nao");
}

// ---------------------------------------------------------------
// Intercalacao e divisao recursiva (base do Merge Sort)
// ---------------------------------------------------------------

// COMBINAR: junta dois vetores JA ORDENADOS em um terceiro ordenado.
// Retorna a quantidade de elementos escritos em saida (na + nb).
int intercalar (const int a[], int na, const int b[], int nb,
                int saida[], Contadores_Merge *c) {
    int i = 0;   // indice de a
    int j = 0;   // indice de b
    int k = 0;   // indice de saida

    // Enquanto as DUAS entradas ainda tem elementos: compara e leva o menor
    while (i < na && j < nb) {
        c->comparacoes++;
        if (a[i] <= b[j]) {       // <= mantem a ordem de a em caso de empate
            saida[k++] = a[i++];
        } else {
            saida[k++] = b[j++];
        }
    }

    // Uma entrada acabou: copia o que sobrou da outra (sem comparar)
    while (i < na) saida[k++] = a[i++];
    while (j < nb) saida[k++] = b[j++];

    return k;
}

void imprimirFaixa (const int v[], int inicio, int fim) {
    for (int i = inicio; i <= fim; i++) printf("%d ", v[i]);
}

// Intervalo [inicio..fim], ambos inclusivos.
void dividirIntervalo (const int v[], int inicio, int fim, int nivel) {
    for (int k = 0; k < nivel; k++) printf("    ");
    printf("[%d..%d]  ", inicio, fim);
    imprimirFaixa(v, inicio, fim);

    // RESOLVER (caso base): intervalo com 1 elemento ja esta ordenado
    if (inicio >= fim) {
        printf(" -> base\n");
        return;
    }

    // DIVIDIR: ponto medio. Escrito assim para nao estourar int com indices
    // grandes (equivale a (inicio + fim) / 2).
    int meio = inicio + (fim - inicio) / 2;
    printf(" -> meio=%d\n", meio);

    // RESOLVER: cada metade e um subproblema menor, resolvido recursivamente
    dividirIntervalo(v, inicio, meio, nivel + 1);       // metade esquerda
    dividirIntervalo(v, meio + 1, fim, nivel + 1);      // metade direita

    // COMBINAR: na proxima atividade, aqui as duas metades ja ordenadas
    // serao juntas com intercalar().
}

bool contemTodos (const int a[], int na, const int b[], int nb,
                  const int saida[], int n) {
    if (n != na + nb) return false;
    bool usado[32] = {false};
    for (int i = 0; i < na + nb; i++) {
        int x = (i < na) ? a[i] : b[i - na];
        bool achou = false;
        for (int k = 0; k < n; k++) {
            if (!usado[k] && saida[k] == x) {
                usado[k] = true;
                achou = true;
                break;
            }
        }
        if (!achou) return false;
    }
    return true;
}

void testarIntercalacao (const char *titulo, const int a[], int na,
                         const int b[], int nb) {
    int saida[32];
    Contadores_Merge c = {0};
    int n = intercalar(a, na, b, nb, saida, &c);

    printf("\n== %s ==\nA: ", titulo);
    for (int i = 0; i < na; i++) printf("%d ", a[i]);
    printf("\nB: ");
    for (int i = 0; i < nb; i++) printf("%d ", b[i]);
    printf("\nSaida: ");
    for (int i = 0; i < n; i++) printf("%d ", saida[i]);

    printf("\nComparacoes=%ld\n", c.comparacoes);
    printf("Entradas ordenadas? %s\n", (estaOrdenado(a, na) && estaOrdenado(b, nb)) ? "sim" : "nao");
    printf("Saida ordenada? %s\n", estaOrdenado(saida, n) ? "sim" : "nao");
    printf("Todos os valores presentes? %s\n", contemTodos(a, na, b, nb, saida, n) ? "sim" : "nao");
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

void Bubble (void) {
    const int ordenado[]  = {5, 17, 17, 30, 42, 63, 88, 91};
    const int inverso[]   = {91, 88, 63, 42, 30, 17, 17, 5};
    const int aleatorio[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int n = sizeof(ordenado) / sizeof(ordenado[0]);

    executarBubble("Vetor ordenado",       ordenado,  n);
    executarBubble("Vetor ordem inversa",  inverso,   n);
    executarBubble("Vetor aleatorio",      aleatorio, n);
}

void BubbleJogadores (void) {
    const Jogador catalogo[] = {
        {7, "Joao", 60}, {1, "Nathan", 80}, {2, "Joao", 95}, {3, "Caua", 80},
        {4, "Rhaic", 70}, {5, "Lucas", 80}, {6, "Paulo", 95}, {8, "Caua", 90}
    };
    const int n = sizeof(catalogo) / sizeof(catalogo[0]);

    const Criterio opcoes[] = {
        {CAMPO_ID, CRESCENTE},        {CAMPO_ID, DECRESCENTE},
        {CAMPO_PONTUACAO, CRESCENTE}, {CAMPO_PONTUACAO, DECRESCENTE},
        {CAMPO_NOME, CRESCENTE},      {CAMPO_NOME, DECRESCENTE}
    };
    const int n_op = sizeof(opcoes) / sizeof(opcoes[0]);

    printf("\n--- Bubble Sort de Jogadores ---\n");
    for (int i = 0; i < n_op; i++) {
        printf("%d. %s (%s)\n", i + 1, nomeCampo(opcoes[i].campo), nomeDirecao(opcoes[i].direcao));
    }
    printf("0. Voltar\nEscolha: ");

    int op;
    if (scanf("%d", &op) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }
    if (op < 1 || op > n_op) return;

    executarBubbleJogadores(catalogo, n, opcoes[op - 1]);
}

void Intercalacao (void) {
    const int a1[] = {5, 17, 42, 88};
    const int b1[] = {8, 17, 30, 91};
    const int a2[] = {1, 2, 3};
    const int b2[] = {10, 20, 30, 40, 50};
    const int a3[] = {50, 60, 70};
    const int b3[] = {1, 2};
    const int a4[] = {7, 9};
    const int vazio[1] = {0};   // usado com tamanho 0

    testarIntercalacao("Intercalada",          a1, 4, b1, 4);
    testarIntercalacao("A termina primeiro",   a2, 3, b2, 5);
    testarIntercalacao("B termina primeiro",   a3, 3, b3, 2);
    testarIntercalacao("Uma entrada vazia",    a4, 2, vazio, 0);

    const int v8[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int v5[] = {9, 4, 7, 1, 6};

    printf("\n== Divisao recursiva (8 elementos) ==\n");
    dividirIntervalo(v8, 0, 7, 0);

    printf("\n== Divisao recursiva (5 elementos, tamanho impar) ==\n");
    dividirIntervalo(v5, 0, 4, 0);
}

int main () {
    int opcao;

    do {
        printf("\n----MENU----\n");
        printf("1.Ordenacao numerica (Selection X Insertion).\n2.Ranking de Jogadores (Selection).\n3.Catalogo por nome (Insertion).\n4.Bubble Sort (ordenado, inverso, aleatorio).\n5.Bubble Sort de Jogadores (campo e direcao).\n6.Intercalacao e divisao recursiva.\n0.Sair.\nEscolha: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1: Numerica();         break;
            case 2: Ranking();          break;
            case 3: CatalogoPorNome();  break;
            case 4: Bubble();           break;
            case 5: BubbleJogadores();  break;
            case 6: Intercalacao();     break;
            case 0:                     break;
            default: printf("Opcao Invalida.\n");
        }
    } while (opcao != 0);
    return 0;
}