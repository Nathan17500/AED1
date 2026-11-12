#include <stdio.h>
#include "missao.h"

static void mostrarResultado (int id_referencia, int id_novo, Resultado_Operacao r) {
    switch (r) {
        case OPERACAO_OK:
            printf("Missao %d inserida apos missao %d.\n", id_novo, id_referencia);
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

    inserirFim(ag, &m1);
    inserirFim(ag, &m2);
    inserirFim(ag, &m3);

    printf("--- Lista inicial ---\n");
    exibirAgenda(ag);

    mostrarResultado(1, 4, inserirApos(ag, 1, &m4));
    mostrarResultado(3, 5, inserirApos(ag, 3, &m5));
    mostrarResultado(99, 6, inserirApos(ag, 99, &m6));

    printf("--- Ordem atual ---\n");
    exibirAgenda(ag);

    Agenda *vazia = criarAgenda();
    if (vazia != NULL) {
        printf("Referencia em lista vazia.\n");
        mostrarResultado(1, 6, inserirApos(vazia, 1, &m6));
        liberarAgenda(vazia);
    }

    liberarAgenda(ag);
return 0;
}