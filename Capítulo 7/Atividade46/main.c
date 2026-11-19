#include <stdio.h>
#include "missao.h"

static void mostrarRemocao (Resultado_Operacao r, const Missao *removida) {
    switch (r) {
        case OPERACAO_OK:
            printf("Removida:");
            exibirMissao(removida);
            break;
        case LISTA_VAZIA:
            printf("Nao ha missao para remover. Lista vazia.\n");
            break;
        case MISSAO_NAO_ENCONTRADA:
            printf("Nenhuma missao corresponde ao identificador informado.\n");
            break;
        case ERRO_ALOCACAO:
        case ARGUMENTO_INVALIDO:
            printf("Argumento invalido.\n");
            break;
    }
}

static void conferir (const Agenda *ag, int esperado) {
    int total = contarMissoes(ag);
    printf("Total: %d (esperado %d) %s\n", total, esperado, total == esperado ? "OK" : "ERRO");
    exibirAgenda(ag);
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
    Missao removida;

    inserirFim(ag, &m1);
    inserirFim(ag, &m2);
    inserirFim(ag, &m3);
    inserirFim(ag, &m4);
    inserirFim(ag, &m5);

    printf("--- Lista inicial ---\n");
    conferir(ag, 5);

    printf("\n--- Remover id 3 (meio) ---\n");
    mostrarRemocao(removerPorId(ag, 3, &removida), &removida);
    conferir(ag, 4);

    printf("\n--- Remover id 1 (inicio) ---\n");
    mostrarRemocao(removerPorId(ag, 1, &removida), &removida);
    conferir(ag, 3);

    printf("\n--- Remover id 5 (ultimo) ---\n");
    mostrarRemocao(removerPorId(ag, 5, &removida), &removida);
    conferir(ag, 2);

    printf("\n--- Remover id 99 (ausente) ---\n");
    mostrarRemocao(removerPorId(ag, 99, &removida), &removida);
    conferir(ag, 2);

    printf("\n--- Remover o fim ---\n");
    mostrarRemocao(removerFim(ag, &removida), &removida);
    conferir(ag, 1);

    printf("\n--- Remover o fim (ultimo elemento) ---\n");
    mostrarRemocao(removerFim(ag, &removida), &removida);
    conferir(ag, 0);

    printf("\n--- Lista vazia ---\n");
    mostrarRemocao(removerFim(ag, &removida), &removida);
    mostrarRemocao(removerPorId(ag, 1, &removida), &removida);
    conferir(ag, 0);

    Agenda *um = criarAgenda();
    if (um != NULL) {
        printf("\n--- Lista com um no ---\n");
        inserirFim(um, &m6);
        conferir(um, 1);

        printf("\n--- Remover id 8 (ausente) ---\n");
        mostrarRemocao(removerPorId(um, 8, &removida), &removida);
        conferir(um, 1);

        printf("\n--- Remover id 6 (unico) ---\n");
        mostrarRemocao(removerPorId(um, 6, &removida), &removida);
        conferir(um, 0);

        liberarAgenda(um);
    }

    liberarAgenda(ag);
return 0;
}