#include <stdio.h>
#include "missao.h"

static void mostrarResultado (const char *relacao, int id_referencia, int id_novo, Resultado_Operacao r) {
    switch (r) {
        case OPERACAO_OK:
            printf("Missao %d inserida %s missao %d.\n", id_novo, relacao ,id_referencia);
            break;
        case MISSAO_NAO_ENCONTRADA:
            printf("Missao %d nao foi encontrada.\n", id_referencia);
            break;
        case ERRO_ALOCACAO:
            printf("Erro na memoria ao inserir a missao.\n");
            break;
        case ARGUMENTO_INVALIDO:
            printf("Argumento invalido.\n");
            break;
        case LISTA_VAZIA:
            printf("Lista vazia.\n");
            break;
    }
}

static void mostrarRemocao (Resultado_Operacao r, const Missao *removida) {
    if (r == OPERACAO_OK) {
        printf("Removida: ");
        exibirMissao (removida);
    } else if (r == LISTA_VAZIA) {
        printf("Nao ha missao para remover. Lista vazia.\n");
    } else {
        printf("Argumento invalido.\n");
    }
}

int main () {
    Agenda *ag = criarAgenda();
    if (ag == NULL) {
        printf("Falha ao criar a agenda.\n");
        return 1;
    }

    Missao m1 = criarMissao(1, "Resgatar o prefeito", 5);
    Missao m2 = criarMissao(2, "Colher flores", 1);
    Missao m3 = criarMissao(3, "Pescar", 2);
    Missao m4 = criarMissao(4, "Defender a cidade", 4);
    Missao m5 = criarMissao(5, "Consertar a ponte", 3);
    Missao m6 = criarMissao(6, "Treinar guardas", 2);
    Missao m7 = criarMissao(7, "Capturar procurado", 4);
    Missao removida;

    inserirFim(ag, &m1);
    inserirFim(ag, &m2);
    inserirFim(ag, &m3);

    printf("--- Lista inicial ---\n");
    exibirAgenda(ag);

    mostrarResultado("antes de", 2, 4, inserirAntes(ag, 2, &m4));
    mostrarResultado("antes de", 1, 5, inserirAntes(ag, 1, &m5));
    mostrarResultado("antes de", 99, 6, inserirAntes(ag, 99, &m6));

    printf("\n--- Ordem atual ---\n");
    exibirAgenda(ag);

    printf("\n--- Remocoes ---\n");
    mostrarRemocao(removerInicio(ag, &removida), &removida);
    mostrarRemocao(removerInicio(ag, &removida), &removida);

    printf("\n--- Apos remocoes ---\n");
    exibirAgenda(ag);

    Agenda *um = criarAgenda();
    if (um != NULL) {
        printf("\n--- Lista com um no ---\n");
        inserirFim(um, &m6);
        mostrarResultado("antes de", 6, 7, inserirAntes(um, 6, &m7));
        exibirAgenda(um);

        mostrarRemocao(removerInicio(um, &removida), &removida);
        mostrarRemocao(removerInicio(um, &removida), &removida);
        mostrarRemocao(removerInicio(um, &removida), &removida);
        exibirAgenda(um);

        liberarAgenda(um);
    }

    liberarAgenda(ag);
return 0;
}