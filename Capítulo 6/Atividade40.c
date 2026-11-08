#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 30

typedef struct Contadores_Merge
{
    long comparacoes;
    long copias;        // elementos copiados (para o auxiliar e de volta)
} Contadores_Merge;

typedef struct Contadores_Quick
{
    long comparacoes;
    long trocas;
} Contadores_Quick;

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
    bool desempateId;   // true: empates no campo sao resolvidos pelo ID crescente
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
    if (crit.direcao == DECRESCENTE) r = -r;
    if (r == 0 && crit.desempateId) {
        // desempate sempre crescente, independente da direcao do campo:
        // com IDs unicos a ordem vira total e todos os algoritmos concordam
        r = (a->id > b->id) - (a->id < b->id);
    }
    return r;
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

// ---------------------------------------------------------------
// Merge Sort: numeros e estruturas compartilham a mesma logica
// ---------------------------------------------------------------

// Regra de comparacao: <0 se a vem antes de b, 0 se empate, >0 se vem depois.
// "ctx" leva dados extras (ex.: o Criterio dos jogadores); pode ser NULL.
typedef int (*Comparador)(const void *a, const void *b, const void *ctx);

int compararInteiros (const void *a, const void *b, const void *ctx) {
    (void)ctx;
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int compararJogadoresGenerico (const void *a, const void *b, const void *ctx) {
    return compararJogadores((const Jogador *)a, (const Jogador *)b, *(const Criterio *)ctx);
}

// COMBINAR: intercala dois blocos ja ordenados (a e b) em saida.
// Funciona para qualquer tipo: "tam" e o tamanho em bytes de cada elemento.
void intercalarGenerico (const void *a, int na, const void *b, int nb,
                         void *saida, size_t tam, Comparador cmp,
                         const void *ctx, Contadores_Merge *c) {
    const unsigned char *pa = a;
    const unsigned char *pb = b;
    unsigned char *ps = saida;
    int i = 0, j = 0, k = 0;

    while (i < na && j < nb) {
        c->comparacoes++;
        if (cmp(pa + (size_t)i * tam, pb + (size_t)j * tam, ctx) <= 0) {
            memcpy(ps + (size_t)k * tam, pa + (size_t)i * tam, tam);   // <= : em empate, vence a esquerda (estavel)
            i++;
        } else {
            memcpy(ps + (size_t)k * tam, pb + (size_t)j * tam, tam);
            j++;
        }
        k++;
    }
    while (i < na) { memcpy(ps + (size_t)k * tam, pa + (size_t)i * tam, tam); i++; k++; }
    while (j < nb) { memcpy(ps + (size_t)k * tam, pb + (size_t)j * tam, tam); j++; k++; }

    c->copias += na + nb;   // todos os elementos foram escritos em saida
}

// Intervalo [inicio..fim], ambos inclusivos. "aux" e o vetor auxiliar,
// com o mesmo tamanho de v, criado uma unica vez por mergeSortGenerico.
static void mergeSortRec (unsigned char *v, unsigned char *aux, size_t tam,
                          int inicio, int fim, Comparador cmp,
                          const void *ctx, Contadores_Merge *c) {
    // CASO-BASE: intervalo com 0 ou 1 elemento ja esta ordenado
    if (inicio >= fim) return;

    // DIVIDIR
    int meio = inicio + (fim - inicio) / 2;

    // RESOLVER: ordena cada metade recursivamente
    mergeSortRec(v, aux, tam, inicio, meio, cmp, ctx, c);
    mergeSortRec(v, aux, tam, meio + 1, fim, cmp, ctx, c);

    // COMBINAR: intercala as duas metades ordenadas em aux e copia de volta
    intercalarGenerico(v + (size_t)inicio * tam,    meio - inicio + 1,
                       v + (size_t)(meio + 1) * tam, fim - meio,
                       aux + (size_t)inicio * tam, tam, cmp, ctx, c);
    memcpy(v + (size_t)inicio * tam, aux + (size_t)inicio * tam,
           (size_t)(fim - inicio + 1) * tam);
    c->copias += fim - inicio + 1;   // copia de volta para v
}

// Retorna false se nao conseguir alocar o vetor auxiliar (v fica intacto).
bool mergeSortGenerico (void *v, int n, size_t tam, Comparador cmp,
                        const void *ctx, Contadores_Merge *c) {
    if (n <= 1) return true;                       // nada a ordenar, sem aux

    unsigned char *aux = malloc((size_t)n * tam);  // um unico auxiliar
    if (aux == NULL) return false;

    mergeSortRec((unsigned char *)v, aux, tam, 0, n - 1, cmp, ctx, c);

    free(aux);                                     // libera em todos os caminhos
    return true;
}

bool mergeSort (int v[], int n, Contadores_Merge *c) {
    return mergeSortGenerico(v, n, sizeof(int), compararInteiros, NULL, c);
}

bool mergeSortJogadores (Jogador v[], int n, Criterio crit, Contadores_Merge *c) {
    return mergeSortGenerico(v, n, sizeof(Jogador), compararJogadoresGenerico, &crit, c);
}

// Entre jogadores empatados no criterio, a ordem original foi mantida?
bool ordemEstavel (const Jogador origem[], const Jogador ordenado[], int n, Criterio crit) {
    for (int i = 1; i < n; i++) {
        if (compararJogadores(&ordenado[i-1], &ordenado[i], crit) == 0) {
            int pa = -1, pb = -1;
            for (int k = 0; k < n; k++) {
                if (origem[k].id == ordenado[i-1].id) pa = k;
                if (origem[k].id == ordenado[i].id)   pb = k;
            }
            if (pa > pb) return false;
        }
    }
    return true;
}

// Aceita ate 32 elementos (limite do contemTodos).
void executarMergeNumeros (const char *titulo, const int original[], int n) {
    if (n > 32) return;
    int v[32];
    memcpy(v, original, n * sizeof(int));

    Contadores_Merge c = {0};
    bool ok = mergeSort(v, n, &c);

    printf("\n== %s ==\n", titulo);
    if (!ok) { printf("Falha ao alocar memoria auxiliar.\n"); return; }

    printf("Antes : ");
    for (int i = 0; i < n; i++) printf("%d ", original[i]);
    printf("\nDepois: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\nComparacoes=%ld\n", c.comparacoes);
    printf("Ordenado? %s\n", estaOrdenado(v, n) ? "sim" : "nao");
    printf("Todos os valores presentes (com repetidos)? %s\n",
           contemTodos(original, n, original, 0, v, n) ? "sim" : "nao");
}

void MergeNumeros (void) {
    const int ordenado[]  = {5, 17, 17, 30, 42, 63, 88, 91};
    const int inverso[]   = {91, 88, 63, 42, 30, 17, 17, 5};
    const int aleatorio[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int repetidos[] = {3, 1, 3, 1, 3, 1, 3};
    const int um[]        = {42};
    const int vazio[1]    = {0};   // usado com tamanho 0

    int grande[30];                // conjunto maior, com valores repetidos
    for (int i = 0; i < 30; i++) grande[i] = (i * 37 + 11) % 20;

    executarMergeNumeros("Vetor ordenado",      ordenado,  8);
    executarMergeNumeros("Vetor ordem inversa", inverso,   8);
    executarMergeNumeros("Vetor aleatorio",     aleatorio, 8);
    executarMergeNumeros("Vetor com repetidos", repetidos, 7);
    executarMergeNumeros("Um elemento",         um,        1);
    executarMergeNumeros("Vetor vazio",         vazio,     0);
    executarMergeNumeros("Vetor maior (30)",    grande,    30);
}

bool escolherCriterio (Criterio *saida) {
    const Criterio opcoes[] = {
        {CAMPO_ID, CRESCENTE, false},        {CAMPO_ID, DECRESCENTE, false},
        {CAMPO_PONTUACAO, CRESCENTE, false}, {CAMPO_PONTUACAO, DECRESCENTE, false},
        {CAMPO_NOME, CRESCENTE, false},      {CAMPO_NOME, DECRESCENTE, false}
    };
    const int n_op = sizeof(opcoes) / sizeof(opcoes[0]);

    for (int i = 0; i < n_op; i++) {
        printf("%d. %s (%s)\n", i + 1, nomeCampo(opcoes[i].campo), nomeDirecao(opcoes[i].direcao));
    }
    printf("0. Voltar\nEscolha: ");

    int op;
    if (scanf("%d", &op) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        return false;
    }
    if (op < 1 || op > n_op) return false;

    *saida = opcoes[op - 1];
    return true;
}

void executarMergeJogadores (const Jogador original[], int n, Criterio crit) {
    Jogador v[16];
    if (n > 16) return;
    memcpy(v, original, n * sizeof(Jogador));

    printf("\nCriterio: %s (%s)\n", nomeCampo(crit.campo), nomeDirecao(crit.direcao));
    exibirJogadores("Antes:", v, n);

    Contadores_Merge c = {0};
    if (!mergeSortJogadores(v, n, crit, &c)) {
        printf("Falha ao alocar memoria auxiliar.\n");
        return;
    }

    exibirJogadores("Depois:", v, n);
    printf("\nComparacoes=%ld\n", c.comparacoes);
    printf("Ordem valida? %s\n", estaOrdenadoJogadores(v, n, crit) ? "sim" : "nao");
    printf("Campos preservados? %s\n", registrosPreservados(original, v, n) ? "sim" : "nao");
    printf("Empates na ordem original (estavel)? %s\n", ordemEstavel(original, v, n, crit) ? "sim" : "nao");
}

void MergeJogadores (void) {
    const Jogador catalogo[] = {
        {7, "Joao", 60}, {1, "Nathan", 80}, {2, "Joao", 95}, {3, "Caua", 80},
        {4, "Rhaic", 70}, {5, "Lucas", 80}, {6, "Paulo", 95}, {8, "Caua", 90}
    };
    const int n = sizeof(catalogo) / sizeof(catalogo[0]);

    printf("\n--- Merge Sort de Jogadores ---\n");
    Criterio crit;
    if (!escolherCriterio(&crit)) return;

    executarMergeJogadores(catalogo, n, crit);
}

// ---------------------------------------------------------------
// Quick Sort: particao com memoria auxiliar x particao in-place
// ---------------------------------------------------------------
// Regra de particao (a mesma nas duas versoes). Pivo = ultimo elemento:
//   esquerda do pivo: valores  <  pivo
//   direita do pivo:  valores >=  pivo   (repetidos do pivo ficam a direita)

static void trocarInteiros (int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Versao com memoria auxiliar: v nao e alterado, o resultado vai para saida.
// Precisa de n >= 1. Retorna a posicao final do pivo em saida.
int particionarAuxiliar (const int v[], int n, int saida[], Contadores_Quick *c) {
    int pivo = v[n - 1];
    int esq = 0;        // proxima posicao livre dos menores (da esquerda p/ direita)
    int dir = n - 1;    // proxima posicao livre dos maiores/iguais (da direita p/ esquerda)

    for (int k = 0; k < n - 1; k++) {
        c->comparacoes++;
        if (v[k] < pivo) saida[esq++] = v[k];
        else             saida[dir--] = v[k];   // lado direito sai em ordem inversa
    }

    saida[esq] = pivo;  // aqui esq == dir: e a posicao final do pivo
    return esq;
}

// Versao in-place (Lomuto) sobre o intervalo [inicio..fim], ambos inclusivos.
// Retorna a posicao final do pivo, sempre entre inicio e fim.
int particionarInPlace (int v[], int inicio, int fim, Contadores_Quick *c) {
    int pivo = v[fim];
    int i = inicio;     // fronteira: v[inicio..i-1] contem so valores < pivo

    for (int j = inicio; j < fim; j++) {
        c->comparacoes++;
        if (v[j] < pivo) {
            if (i != j) {
                trocarInteiros(&v[i], &v[j]);
                c->trocas++;
            }
            i++;
        }
    }

    if (i != fim) {     // coloca o pivo na posicao final
        trocarInteiros(&v[i], &v[fim]);
        c->trocas++;
    }
    return i;
}

// A particao em [inicio..fim] respeita a regra em torno da posicao p?
bool particaoValida (const int v[], int inicio, int fim, int p, int pivo) {
    if (p < inicio || p > fim || v[p] != pivo) return false;
    for (int i = inicio; i < p; i++)  if (v[i] >= pivo) return false;
    for (int i = p + 1; i <= fim; i++) if (v[i] < pivo)  return false;
    return true;
}

// Quick Sort: ordena v[inicio..fim] usando a particao in-place.
static void quickSortRec (int v[], int inicio, int fim, int nivel,
                          bool mostrar, Contadores_Quick *c) {
    // CASO-BASE: 0 ou 1 elemento ja esta ordenado
    if (inicio >= fim) return;

    int pivo = v[fim];
    int p = particionarInPlace(v, inicio, fim, c);   // pivo ja esta na posicao final

    if (mostrar) {
        for (int k = 0; k < nivel; k++) printf("    ");
        printf("[%d..%d] pivo=%d -> posicao final=%d | ", inicio, fim, pivo, p);
        imprimirFaixa(v, inicio, fim);
        printf("\n");
    }

    // So desce em subintervalos com 2 ou mais elementos; o pivo (p) fica de fora
    if (p - 1 > inicio) quickSortRec(v, inicio, p - 1, nivel + 1, mostrar, c);
    if (p + 1 < fim)    quickSortRec(v, p + 1, fim,    nivel + 1, mostrar, c);
}

void quickSort (int v[], int n, bool mostrar, Contadores_Quick *c) {
    if (n > 1) quickSortRec(v, 0, n - 1, 0, mostrar, c);
}

// Demonstra as duas particoes isoladamente (ate 32 elementos).
void testarParticao (const char *titulo, const int original[], int n) {
    if (n < 1 || n > 32) return;
    int pivo = original[n - 1];
    int aux[32];
    int local[32];
    memcpy(local, original, n * sizeof(int));

    Contadores_Quick ca = {0, 0};
    Contadores_Quick ci = {0, 0};
    int pos_a = particionarAuxiliar(original, n, aux, &ca);
    int pos_i = particionarInPlace(local, 0, n - 1, &ci);

    printf("\n== %s ==\nOriginal : ", titulo);
    imprimirFaixa(original, 0, n - 1);
    printf("\nPivo (ultimo elemento) = %d\n", pivo);

    printf("Auxiliar : ");
    imprimirFaixa(aux, 0, n - 1);
    printf("\n   posicao final do pivo=%d | comparacoes=%ld\n", pos_a, ca.comparacoes);

    printf("In-place : ");
    imprimirFaixa(local, 0, n - 1);
    printf("\n   posicao final do pivo=%d | comparacoes=%ld trocas=%ld\n",
           pos_i, ci.comparacoes, ci.trocas);

    printf("Regra (< pivo a esquerda, >= pivo a direita)? auxiliar: %s | in-place: %s\n",
           particaoValida(aux, 0, n - 1, pos_a, pivo) ? "sim" : "nao",
           particaoValida(local, 0, n - 1, pos_i, pivo) ? "sim" : "nao");
    printf("Todos os elementos preservados? auxiliar: %s | in-place: %s\n",
           contemTodos(original, n, original, 0, aux, n) ? "sim" : "nao",
           contemTodos(original, n, original, 0, local, n) ? "sim" : "nao");
    printf("Mesma posicao do pivo nas duas versoes? %s\n", pos_a == pos_i ? "sim" : "nao");
}

void Particoes (void) {
    const int aleatorio[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int ordenado[]  = {5, 17, 17, 30, 42, 63, 88, 91};   // pivo = maior
    const int inverso[]   = {91, 88, 63, 42, 30, 17, 17, 5};   // pivo = menor
    const int repetidos[] = {3, 1, 3, 1, 3, 1, 3};
    const int iguais[]    = {5, 5, 5, 5, 5};
    const int um[]        = {42};

    testarParticao("Aleatorio",              aleatorio, 8);
    testarParticao("Ordenado (pivo maior)",  ordenado,  8);
    testarParticao("Inverso (pivo menor)",   inverso,   8);
    testarParticao("Com repetidos do pivo",  repetidos, 7);
    testarParticao("Todos iguais",           iguais,    5);
    testarParticao("Um elemento",            um,        1);
}

// Aceita ate 32 elementos (limite do contemTodos).
void executarQuick (const char *titulo, const int original[], int n, bool mostrar) {
    if (n > 32) return;
    int v[32];
    memcpy(v, original, n * sizeof(int));

    printf("\n== %s ==\nAntes : ", titulo);
    for (int i = 0; i < n; i++) printf("%d ", original[i]);
    printf("\n");

    Contadores_Quick c = {0, 0};
    if (mostrar) printf("Passos (intervalo, pivo e posicao final):\n");
    quickSort(v, n, mostrar, &c);

    printf("Depois: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\nComparacoes=%ld  Trocas=%ld\n", c.comparacoes, c.trocas);
    printf("Ordenado? %s\n", estaOrdenado(v, n) ? "sim" : "nao");
    printf("Todos os valores presentes (com repetidos)? %s\n",
           contemTodos(original, n, original, 0, v, n) ? "sim" : "nao");
}

void QuickNumeros (void) {
    const int aleatorio[] = {42, 17, 88, 5, 63, 17, 91, 30};
    const int ordenado[]  = {5, 17, 17, 30, 42, 63, 88, 91};
    const int inverso[]   = {91, 88, 63, 42, 30, 17, 17, 5};
    const int repetidos[] = {3, 1, 3, 1, 3, 1, 3};
    const int iguais[]    = {5, 5, 5, 5, 5};
    const int um[]        = {42};
    const int vazio[1]    = {0};   // usado com tamanho 0

    int grande[30];                // conjunto maior, com valores repetidos
    for (int i = 0; i < 30; i++) grande[i] = (i * 37 + 11) % 20;

    executarQuick("Vetor aleatorio (com passos)", aleatorio, 8, true);
    executarQuick("Vetor ordenado",       ordenado,  8,  false);
    executarQuick("Vetor ordem inversa",  inverso,   8,  false);
    executarQuick("Vetor com repetidos",  repetidos, 7,  false);
    executarQuick("Todos iguais",         iguais,    5,  false);
    executarQuick("Um elemento",          um,        1,  false);
    executarQuick("Vetor vazio",          vazio,     0,  false);
    executarQuick("Vetor maior (30)",     grande,    30, false);
}

// ---------------------------------------------------------------
// Quick Sort com pivo aleatorio e suporte a jogadores
// ---------------------------------------------------------------

// Sorteio uniforme em [minimo..maximo]. O gerador (srand) e inicializado uma
// unica vez, em main. Adequado para intervalos pequenos (ate RAND_MAX).
static int sortear (int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

static void quickSortAleatorioRec (int v[], int inicio, int fim, Contadores_Quick *c) {
    // CASO-BASE: 0 ou 1 elemento
    if (inicio >= fim) return;

    // Pivo sorteado SOMENTE dentro de [inicio..fim] e levado para v[fim],
    // a posicao que particionarInPlace espera. A troca conta como troca.
    int r = sortear(inicio, fim);
    if (r != fim) {
        trocarInteiros(&v[r], &v[fim]);
        c->trocas++;
    }

    int p = particionarInPlace(v, inicio, fim, c);

    if (p - 1 > inicio) quickSortAleatorioRec(v, inicio, p - 1, c);
    if (p + 1 < fim)    quickSortAleatorioRec(v, p + 1, fim, c);
}

void quickSortAleatorio (int v[], int n, Contadores_Quick *c) {
    if (n > 1) quickSortAleatorioRec(v, 0, n - 1, c);
}

static void trocarJogadores (Jogador *a, Jogador *b) {
    Jogador tmp = *a;     // copia a struct inteira (id, nome e pontuacao)
    *a = *b;
    *b = tmp;
}

// Mesma regra da particao de inteiros, agora com o criterio explicito:
// "antes do pivo" = compararJogadores(...) < 0 ; pivo = v[fim].
int particionarJogadores (Jogador v[], int inicio, int fim, Criterio crit,
                          Contadores_Quick *c) {
    Jogador pivo = v[fim];    // copia do pivo: v[fim] nao muda durante o laco
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        c->comparacoes++;
        if (compararJogadores(&v[j], &pivo, crit) < 0) {
            if (i != j) {
                trocarJogadores(&v[i], &v[j]);
                c->trocas++;
            }
            i++;
        }
    }

    if (i != fim) {
        trocarJogadores(&v[i], &v[fim]);
        c->trocas++;
    }
    return i;
}

static void quickSortJogadoresRec (Jogador v[], int inicio, int fim, Criterio crit,
                                   bool aleatorio, Contadores_Quick *c) {
    if (inicio >= fim) return;

    if (aleatorio) {
        int r = sortear(inicio, fim);      // so dentro do subintervalo atual
        if (r != fim) {
            trocarJogadores(&v[r], &v[fim]);
            c->trocas++;
        }
    }

    int p = particionarJogadores(v, inicio, fim, crit, c);

    if (p - 1 > inicio) quickSortJogadoresRec(v, inicio, p - 1, crit, aleatorio, c);
    if (p + 1 < fim)    quickSortJogadoresRec(v, p + 1, fim, crit, aleatorio, c);
}

void quickSortJogadores (Jogador v[], int n, Criterio crit, bool aleatorio,
                         Contadores_Quick *c) {
    if (n > 1) quickSortJogadoresRec(v, 0, n - 1, crit, aleatorio, c);
}

// Selection e Insertion de jogadores com o mesmo criterio explicito
// (as versoes anteriores, com regra fixa, continuam no codigo).
void selectionSortJogadoresCriterio (Jogador v[], int n, Criterio crit,
                                     Contadores_Selection *c) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            c->comparacoes++;
            if (compararJogadores(&v[j], &v[menor], crit) < 0) menor = j;
        }
        if (menor != i) {
            trocarJogadores(&v[i], &v[menor]);
            c->trocas++;
        }
    }
}

void insertionSortJogadoresCriterio (Jogador v[], int n, Criterio crit,
                                     Contadores_Insertion *c) {
    for (int i = 1; i < n; i++) {
        Jogador chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            c->comparacoes++;
            if (compararJogadores(&chave, &v[j], crit) < 0) {
                v[j + 1] = v[j];
                c->deslocamentos++;
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }
}

// ---------------------------------------------------------------
// Metricas comparaveis e despacho dos algoritmos
// ---------------------------------------------------------------

// -1 significa "nao se aplica" ao algoritmo.
typedef struct Metricas
{
    long comparacoes;
    long trocas;
    long deslocamentos;
} Metricas;

typedef enum {
    ALG_SELECTION,
    ALG_INSERTION,
    ALG_BUBBLE,
    ALG_MERGE,
    ALG_QUICK_FIXO,
    ALG_QUICK_ALEATORIO,
    TOTAL_ALG
} Algoritmo;

const char *nomeAlgoritmo (Algoritmo alg) {
    switch (alg) {
        case ALG_SELECTION:       return "Selection Sort";
        case ALG_INSERTION:       return "Insertion Sort";
        case ALG_BUBBLE:          return "Bubble Sort";
        case ALG_MERGE:           return "Merge Sort";
        case ALG_QUICK_FIXO:      return "Quick Sort (pivo fixo)";
        case ALG_QUICK_ALEATORIO: return "Quick Sort (pivo aleatorio)";
        default:                  return "?";
    }
}

// Ordena v (ja uma COPIA) com o algoritmo escolhido. false = falha de memoria.
bool ordenarNumeros (Algoritmo alg, int v[], int n, Metricas *m) {
    m->comparacoes = m->trocas = m->deslocamentos = -1;

    switch (alg) {
        case ALG_SELECTION: {
            Contadores_Selection c = {0, 0};
            selectionSort(v, n, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        case ALG_INSERTION: {
            Contadores_Insertion c = {0, 0};
            insertionSort(v, n, &c);
            m->comparacoes = c.comparacoes;
            m->deslocamentos = c.deslocamentos;
            break;
        }
        case ALG_BUBBLE: {
            Contadores_Bubble c = {0, 0, 0};
            bubbleSort(v, n, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        case ALG_MERGE: {
            Contadores_Merge c = {0};
            if (!mergeSort(v, n, &c)) return false;
            m->comparacoes = c.comparacoes;
            m->deslocamentos = c.copias;
            break;
        }
        case ALG_QUICK_FIXO: {
            Contadores_Quick c = {0, 0};
            quickSort(v, n, false, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        case ALG_QUICK_ALEATORIO: {
            Contadores_Quick c = {0, 0};
            quickSortAleatorio(v, n, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        default:
            return false;
    }
    return true;
}

bool ordenarJogadores (Algoritmo alg, Jogador v[], int n, Criterio crit, Metricas *m) {
    m->comparacoes = m->trocas = m->deslocamentos = -1;

    switch (alg) {
        case ALG_SELECTION: {
            Contadores_Selection c = {0, 0};
            selectionSortJogadoresCriterio(v, n, crit, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        case ALG_INSERTION: {
            Contadores_Insertion c = {0, 0};
            insertionSortJogadoresCriterio(v, n, crit, &c);
            m->comparacoes = c.comparacoes;
            m->deslocamentos = c.deslocamentos;
            break;
        }
        case ALG_BUBBLE: {
            Contadores_Bubble c = {0, 0, 0};
            bubbleSortJogadores(v, n, crit, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        case ALG_MERGE: {
            Contadores_Merge c = {0};
            if (!mergeSortJogadores(v, n, crit, &c)) return false;
            m->comparacoes = c.comparacoes;
            m->deslocamentos = c.copias;
            break;
        }
        case ALG_QUICK_FIXO:
        case ALG_QUICK_ALEATORIO: {
            Contadores_Quick c = {0, 0};
            quickSortJogadores(v, n, crit, alg == ALG_QUICK_ALEATORIO, &c);
            m->comparacoes = c.comparacoes;
            m->trocas = c.trocas;
            break;
        }
        default:
            return false;
    }
    return true;
}

// ---------------------------------------------------------------
// Conferencias
// ---------------------------------------------------------------

static int compararInteirosQsort (const void *a, const void *b) {
    return compararInteiros(a, b, NULL);
}

// A saida confere com a entrada original? Ordena uma copia da original com
// qsort e compara: garante ordem correta E os mesmos elementos (com repetidos).
bool numerosConferem (const int original[], const int ordenado[], int n) {
    if (n <= 0) return true;
    int *ref = malloc((size_t)n * sizeof(int));
    if (ref == NULL) return false;
    memcpy(ref, original, (size_t)n * sizeof(int));
    qsort(ref, (size_t)n, sizeof(int), compararInteirosQsort);
    bool ok = memcmp(ref, ordenado, (size_t)n * sizeof(int)) == 0;
    free(ref);
    return ok;
}

bool jogadoresIguais (const Jogador a[], const Jogador b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i].id != b[i].id || a[i].pontuacao != b[i].pontuacao ||
            strcmp(a[i].nome, b[i].nome) != 0) return false;
    }
    return true;
}

// ---------------------------------------------------------------
// Central de ordenacao: dados, menu e relatorio
// ---------------------------------------------------------------

#define MAX_CENTRAL 2000

typedef struct DadosCentral
{
    int *numeros;
    int n_num;
    Jogador *jogadores;
    int n_jog;
} DadosCentral;

bool lerInteiro (const char *pergunta, int minimo, int maximo, int *valor) {
    printf("%s (%d a %d): ", pergunta, minimo, maximo);
    if (scanf("%d", valor) != 1) {
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Entrada invalida.\n");
        return false;
    }
    if (*valor < minimo || *valor > maximo) {
        printf("Valor fora do intervalo.\n");
        return false;
    }
    return true;
}

void gerarNumeros (DadosCentral *d) {
    int n, formato;
    if (!lerInteiro("Quantidade de numeros", 1, MAX_CENTRAL, &n)) return;
    printf("Formato: 1.Aleatorio 2.Crescente 3.Decrescente 4.Muitos repetidos\n");
    if (!lerInteiro("Formato", 1, 4, &formato)) return;

    int *novo = malloc((size_t)n * sizeof(int));
    if (novo == NULL) { printf("Falha ao alocar memoria.\n"); return; }

    int maximo = (formato == 4) ? 4 : 999;
    for (int i = 0; i < n; i++) novo[i] = sortear(0, maximo);

    if (formato == 2 || formato == 3) {
        qsort(novo, (size_t)n, sizeof(int), compararInteirosQsort);
        if (formato == 3) {
            for (int i = 0; i < n / 2; i++) trocarInteiros(&novo[i], &novo[n - 1 - i]);
        }
    }

    free(d->numeros);
    d->numeros = novo;
    d->n_num = n;
    printf("%d numeros gerados.\n", n);
}

void carregarNumeros (DadosCentral *d) {
    char caminho[256];
    printf("Arquivo de numeros (inteiros separados por espaco ou linha): ");
    if (scanf(" %255s", caminho) != 1) return;

    FILE *f = fopen(caminho, "r");
    if (f == NULL) { printf("Nao foi possivel abrir '%s'.\n", caminho); return; }

    int *novo = malloc(MAX_CENTRAL * sizeof(int));
    if (novo == NULL) { fclose(f); printf("Falha ao alocar memoria.\n"); return; }

    int n = 0, x;
    while (n < MAX_CENTRAL && fscanf(f, "%d", &x) == 1) novo[n++] = x;
    bool truncado = (n == MAX_CENTRAL && fscanf(f, "%d", &x) == 1);
    fclose(f);

    if (n == 0) {
        free(novo);
        printf("Nenhum inteiro valido no arquivo.\n");
        return;
    }

    free(d->numeros);
    d->numeros = novo;
    d->n_num = n;
    printf("%d numeros carregados.%s\n", n,
           truncado ? " (excedente do limite foi ignorado)" : "");
}

void gerarJogadores (DadosCentral *d) {
    static const char *nomes[] = {"Nathan", "Joao", "Caua", "Rhaic", "Lucas",
                                  "Paulo", "Ana", "Bia", "Davi", "Livia"};
    const int n_nomes = sizeof(nomes) / sizeof(nomes[0]);

    int n;
    if (!lerInteiro("Quantidade de jogadores", 1, MAX_CENTRAL, &n)) return;

    Jogador *novo = malloc((size_t)n * sizeof(Jogador));
    if (novo == NULL) { printf("Falha ao alocar memoria.\n"); return; }

    for (int i = 0; i < n; i++) {
        novo[i].id = i + 1;
        strcpy(novo[i].nome, nomes[sortear(0, n_nomes - 1)]);   // nomes repetem
        novo[i].pontuacao = sortear(0, 100);                    // pontos repetem
    }
    // embaralha os IDs: a ordem de cadastro nao coincide com a ordem por ID
    for (int i = n - 1; i > 0; i--) {
        int j = sortear(0, i);
        int tmp = novo[i].id;
        novo[i].id = novo[j].id;
        novo[j].id = tmp;
    }

    free(d->jogadores);
    d->jogadores = novo;
    d->n_jog = n;
    printf("%d jogadores gerados.\n", n);
}

void carregarJogadores (DadosCentral *d) {
    char caminho[256];
    printf("Arquivo de jogadores (uma linha por jogador: id nome pontuacao): ");
    if (scanf(" %255s", caminho) != 1) return;

    FILE *f = fopen(caminho, "r");
    if (f == NULL) { printf("Nao foi possivel abrir '%s'.\n", caminho); return; }

    Jogador *novo = malloc(MAX_CENTRAL * sizeof(Jogador));
    if (novo == NULL) { fclose(f); printf("Falha ao alocar memoria.\n"); return; }

    int n = 0;
    int id_repetido = 0;
    Jogador j;
    while (n < MAX_CENTRAL &&
           fscanf(f, "%d %29s %d", &j.id, j.nome, &j.pontuacao) == 3) {
        for (int k = 0; k < n; k++) {
            if (novo[k].id == j.id) id_repetido = j.id;
        }
        if (id_repetido) break;
        novo[n++] = j;
    }
    fclose(f);

    if (id_repetido) {
        free(novo);
        printf("ID repetido (%d): os IDs precisam ser unicos. Nada foi carregado.\n", id_repetido);
        return;
    }
    if (n == 0) {
        free(novo);
        printf("Nenhum jogador valido no arquivo.\n");
        return;
    }

    free(d->jogadores);
    d->jogadores = novo;
    d->n_jog = n;
    printf("%d jogadores carregados.\n", n);
}

void resumoDados (const DadosCentral *d) {
    printf("Numeros   : ");
    if (d->n_num == 0) {
        printf("(nenhum)\n");
    } else {
        printf("%d elementos", d->n_num);
        if (d->n_num <= 20) { printf(": "); imprimirFaixa(d->numeros, 0, d->n_num - 1); }
        printf("\n");
    }
    printf("Jogadores : ");
    if (d->n_jog == 0) printf("(nenhum)\n");
    else               printf("%d jogadores\n", d->n_jog);
}

bool escolherAlgoritmo (Algoritmo *alg) {
    printf("\nAlgoritmos:\n");
    for (int i = 0; i < TOTAL_ALG; i++) printf("%d. %s\n", i + 1, nomeAlgoritmo((Algoritmo)i));
    printf("0. Voltar\n");

    int op;
    if (!lerInteiro("Algoritmo", 0, TOTAL_ALG, &op) || op == 0) return false;
    *alg = (Algoritmo)(op - 1);
    return true;
}

static void textoMetrica (long valor, char *buf, size_t tam) {
    if (valor < 0) snprintf(buf, tam, "-");
    else           snprintf(buf, tam, "%ld", valor);
}

void imprimirCabecalhoMetricas (void) {
    printf("%-28s %12s %10s %14s   %s\n",
           "Algoritmo", "Comparacoes", "Trocas", "Deslocamentos", "Confere");
    printf("--------------------------------------------------------------------------\n");
}

void imprimirLinhaMetricas (const char *nome, const Metricas *m, bool confere) {
    char a[24], b[24], c[24];
    textoMetrica(m->comparacoes, a, sizeof a);
    textoMetrica(m->trocas, b, sizeof b);
    textoMetrica(m->deslocamentos, c, sizeof c);
    printf("%-28s %12s %10s %14s   %s\n", nome, a, b, c, confere ? "sim" : "NAO");
}

void imprimirNotasMetricas (bool jogadores) {
    printf("\nNotas:\n");
    printf(" - Trocas: dois elementos trocam de lugar (no Quick inclui a troca do pivo sorteado).\n");
    printf(" - Deslocamentos: Insertion = elementos empurrados uma posicao; Merge = elementos\n");
    printf("   copiados (para o vetor auxiliar e de volta).\n");
    printf(" - \"-\" = metrica que nao se aplica ao algoritmo.\n");
    if (jogadores) {
        printf(" - Confere: ordem valida pelo criterio, registros preservados e resultado\n");
        printf("   identico ao do primeiro algoritmo (o desempate por ID torna a ordem unica).\n");
    } else {
        printf(" - Confere: saida ordenada e com os mesmos elementos da entrada (com repetidos).\n");
    }
}

void ordenarNumerosCentral (const DadosCentral *d) {
    if (d->n_num == 0) { printf("Gere ou carregue numeros primeiro.\n"); return; }

    Algoritmo alg;
    if (!escolherAlgoritmo(&alg)) return;

    int n = d->n_num;
    int *copia = malloc((size_t)n * sizeof(int));
    if (copia == NULL) { printf("Falha ao alocar memoria.\n"); return; }
    memcpy(copia, d->numeros, (size_t)n * sizeof(int));   // a original fica intacta

    Metricas m;
    if (!ordenarNumeros(alg, copia, n, &m)) {
        printf("Falha ao executar o algoritmo (memoria).\n");
        free(copia);
        return;
    }

    printf("\n== %s sobre uma copia de %d numeros ==\n", nomeAlgoritmo(alg), n);
    if (n <= 20) {
        printf("Antes : "); imprimirFaixa(d->numeros, 0, n - 1);
        printf("\nDepois: "); imprimirFaixa(copia, 0, n - 1);
        printf("\n");
    }
    printf("\n");
    imprimirCabecalhoMetricas();
    imprimirLinhaMetricas(nomeAlgoritmo(alg), &m, numerosConferem(d->numeros, copia, n));
    imprimirNotasMetricas(false);
    free(copia);
}

void ordenarJogadoresCentral (const DadosCentral *d) {
    if (d->n_jog == 0) { printf("Gere ou carregue jogadores primeiro.\n"); return; }

    Algoritmo alg;
    if (!escolherAlgoritmo(&alg)) return;
    printf("\nCriterio de ordenacao:\n");
    Criterio crit;
    if (!escolherCriterio(&crit)) return;
    crit.desempateId = true;

    int n = d->n_jog;
    Jogador *copia = malloc((size_t)n * sizeof(Jogador));
    if (copia == NULL) { printf("Falha ao alocar memoria.\n"); return; }
    memcpy(copia, d->jogadores, (size_t)n * sizeof(Jogador));

    printf("\n== %s sobre uma copia de %d jogadores ==\n", nomeAlgoritmo(alg), n);
    printf("Criterio: %s (%s), desempate: ID crescente\n",
           nomeCampo(crit.campo), nomeDirecao(crit.direcao));
    if (n <= 12) exibirJogadores("Antes:", copia, n);

    Metricas m;
    if (!ordenarJogadores(alg, copia, n, crit, &m)) {
        printf("Falha ao executar o algoritmo (memoria).\n");
        free(copia);
        return;
    }

    if (n <= 12) exibirJogadores("Depois:", copia, n);

    bool confere = estaOrdenadoJogadores(copia, n, crit) &&
                   registrosPreservados(d->jogadores, copia, n);
    printf("\n");
    imprimirCabecalhoMetricas();
    imprimirLinhaMetricas(nomeAlgoritmo(alg), &m, confere);
    printf("\nNota: confere = ordem valida pelo criterio e registros preservados.\n");
    free(copia);
}

void relatorioNumeros (const DadosCentral *d) {
    if (d->n_num == 0) { printf("Gere ou carregue numeros primeiro.\n"); return; }

    int n = d->n_num;
    int *copia = malloc((size_t)n * sizeof(int));
    if (copia == NULL) { printf("Falha ao alocar memoria.\n"); return; }

    printf("\n=== RELATORIO COMPARATIVO: %d numeros (mesma entrada original para todos) ===\n\n", n);
    imprimirCabecalhoMetricas();
    for (int a = 0; a < TOTAL_ALG; a++) {
        memcpy(copia, d->numeros, (size_t)n * sizeof(int));   // sempre parte da original
        Metricas m;
        if (!ordenarNumeros((Algoritmo)a, copia, n, &m)) {
            printf("%-28s falha de memoria\n", nomeAlgoritmo((Algoritmo)a));
            continue;
        }
        imprimirLinhaMetricas(nomeAlgoritmo((Algoritmo)a), &m,
                              numerosConferem(d->numeros, copia, n));
    }
    imprimirNotasMetricas(false);
    free(copia);
}

void relatorioJogadores (const DadosCentral *d) {
    if (d->n_jog == 0) { printf("Gere ou carregue jogadores primeiro.\n"); return; }

    printf("\nCriterio de ordenacao:\n");
    Criterio crit;
    if (!escolherCriterio(&crit)) return;
    crit.desempateId = true;

    int n = d->n_jog;
    Jogador *copia = malloc((size_t)n * sizeof(Jogador));
    Jogador *ref = malloc((size_t)n * sizeof(Jogador));   // resultado do 1o algoritmo
    if (copia == NULL || ref == NULL) {
        free(copia); free(ref);
        printf("Falha ao alocar memoria.\n");
        return;
    }

    printf("\n=== RELATORIO COMPARATIVO: %d jogadores (mesma entrada original para todos) ===\n", n);
    printf("Criterio: %s (%s), desempate: ID crescente\n\n",
           nomeCampo(crit.campo), nomeDirecao(crit.direcao));
    imprimirCabecalhoMetricas();

    for (int a = 0; a < TOTAL_ALG; a++) {
        memcpy(copia, d->jogadores, (size_t)n * sizeof(Jogador));   // sempre a original
        Metricas m;
        if (!ordenarJogadores((Algoritmo)a, copia, n, crit, &m)) {
            printf("%-28s falha de memoria\n", nomeAlgoritmo((Algoritmo)a));
            continue;
        }
        if (a == 0) memcpy(ref, copia, (size_t)n * sizeof(Jogador));

        bool confere = estaOrdenadoJogadores(copia, n, crit) &&
                       registrosPreservados(d->jogadores, copia, n) &&
                       jogadoresIguais(ref, copia, n);
        imprimirLinhaMetricas(nomeAlgoritmo((Algoritmo)a), &m, confere);
    }
    imprimirNotasMetricas(true);
    free(copia);
    free(ref);
}

void CentralOrdenacao (void) {
    DadosCentral d = {NULL, 0, NULL, 0};
    int op;

    do {
        printf("\n--- CENTRAL DE ORDENACAO ---\n");
        resumoDados(&d);
        printf("1.Gerar numeros aleatorios\n2.Carregar numeros de arquivo\n");
        printf("3.Gerar jogadores aleatorios\n4.Carregar jogadores de arquivo\n");
        printf("5.Ordenar numeros (um algoritmo)\n6.Ordenar jogadores (um algoritmo)\n");
        printf("7.Relatorio comparativo - numeros\n8.Relatorio comparativo - jogadores\n");
        printf("0.Voltar\nEscolha: ");
        if (scanf("%d", &op) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF);
            break;
        }

        switch (op) {
            case 1: gerarNumeros(&d);             break;
            case 2: carregarNumeros(&d);          break;
            case 3: gerarJogadores(&d);           break;
            case 4: carregarJogadores(&d);        break;
            case 5: ordenarNumerosCentral(&d);    break;
            case 6: ordenarJogadoresCentral(&d);  break;
            case 7: relatorioNumeros(&d);         break;
            case 8: relatorioJogadores(&d);       break;
            case 0:                               break;
            default: printf("Opcao Invalida.\n");
        }
    } while (op != 0);

    free(d.numeros);       // libera os dados da central ao sair
    free(d.jogadores);
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
        {CAMPO_ID, CRESCENTE, false},        {CAMPO_ID, DECRESCENTE, false},
        {CAMPO_PONTUACAO, CRESCENTE, false}, {CAMPO_PONTUACAO, DECRESCENTE, false},
        {CAMPO_NOME, CRESCENTE, false},      {CAMPO_NOME, DECRESCENTE, false}
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

int main (int argc, char *argv[]) {
    int opcao;

    // Gerador pseudoaleatorio inicializado UMA unica vez, aqui no inicio.
    // Semente opcional em argv[1] para repetir uma execucao.
    unsigned int semente = (argc > 1) ? (unsigned int)strtoul(argv[1], NULL, 10)
                                      : (unsigned int)time(NULL);
    srand(semente);
    printf("Semente do gerador: %u\n", semente);

    do {
        printf("\n----MENU----\n");
        printf("1.Ordenacao numerica (Selection X Insertion).\n2.Ranking de Jogadores (Selection).\n3.Catalogo por nome (Insertion).\n4.Bubble Sort (ordenado, inverso, aleatorio).\n5.Bubble Sort de Jogadores (campo e direcao).\n6.Intercalacao e divisao recursiva.\n7.Merge Sort numerico.\n8.Merge Sort de Jogadores (campo e direcao).\n9.Particao (auxiliar x in-place).\n10.Quick Sort.\n11.Central de ordenacao (gerar/carregar dados e comparar algoritmos).\n0.Sair.\nEscolha: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1: Numerica();         break;
            case 2: Ranking();          break;
            case 3: CatalogoPorNome();  break;
            case 4: Bubble();           break;
            case 5: BubbleJogadores();  break;
            case 6: Intercalacao();     break;
            case 7: MergeNumeros();     break;
            case 8: MergeJogadores();   break;
            case 9: Particoes();        break;
            case 10: QuickNumeros();    break;
            case 11: CentralOrdenacao(); break;
            case 0:                     break;
            default: printf("Opcao Invalida.\n");
        }
    } while (opcao != 0);
    return 0;
}