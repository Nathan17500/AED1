#include <stdio.h>
#include "missao.h"

static void mostrarRemocao (Resultado_Operacao r, const Missao *removida) {
    switch (r) {
        case OPERACAO_OK:
            printf("Removida: ");
            exibirMissao(removida);
            break;
        case LISTA_VAZIA:
            printf("Nao ha missao para remover. Lista vazia.\n");
            break;
        case MISSAO_NAO_ENCONTRADA:
            printf("Nenhuma missao corresponde ao identificador informado.\n");
            break;
        case SEM_SUCESSOR:
            printf("Nao existe missao depois da referencia.\n");
            break;
        case SEM_ANTECESSOR:
            printf("Nao existe missao antes da referencia.\n");
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

int main (void) {
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
    Missao removida;

    inserirFim(ag, &m1);
    inserirFim(ag, &m2);
    inserirFim(ag, &m3);
    inserirFim(ag, &m4);
    inserirFim(ag, &m5);

    printf("--- Lista inicial ---\n");
    conferir(ag, 5);

    printf("\n--- Remover antes do id 4 (meio) ---\n");
    mostrarRemocao(removerAntes(ag, 4, &removida), &removida);
    conferir(ag, 4);

    printf("\n--- Remover antes do id 2 (alvo e o primeiro) ---\n");
    mostrarRemocao(removerAntes(ag, 2, &removida), &removida);
    conferir(ag, 3);

    printf("\n--- Remover antes do id 2 (ref e o primeiro) ---\n");
    mostrarRemocao(removerAntes(ag, 2, &removida), &removida);
    conferir(ag, 3);

    printf("\n--- Remover antes do id 99 (ausente) ---\n");
    mostrarRemocao(removerAntes(ag, 99, &removida), &removida);
    conferir(ag, 3);

    printf("\n--- Remover depois do id 4 (alvo e o ultimo) ---\n");
    mostrarRemocao(removerApos(ag, 4, &removida), &removida);
    conferir(ag, 2);

    printf("\n--- Remover depois do id 4 (ref e o ultimo) ---\n");
    mostrarRemocao(removerApos(ag, 4, &removida), &removida);
    conferir(ag, 2);

    printf("\n--- Remover depois do id 99 (ausente) ---\n");
    mostrarRemocao(removerApos(ag, 99, &removida), &removida);
    conferir(ag, 2);

    printf("\n--- Remover depois do id 2 ---\n");
    mostrarRemocao(removerApos(ag, 2, &removida), &removida);
    conferir(ag, 1);

    printf("\n--- Um unico no ---\n");
    mostrarRemocao(removerApos(ag, 2, &removida), &removida);
    mostrarRemocao(removerAntes(ag, 2, &removida), &removida);
    conferir(ag, 1);

    printf("\n--- Lista vazia ---\n");
    mostrarRemocao(removerInicio(ag, &removida), &removida);
    mostrarRemocao(removerApos(ag, 2, &removida), &removida);
    mostrarRemocao(removerAntes(ag, 2, &removida), &removida);
    conferir(ag, 0);

    liberarAgenda(ag);
    return 0;
}