#include <stdio.h>
#include "missao.h"

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

    printf("--- Lista ---\n");
    exibirAgenda(ag);

    inserirFim(ag, &m1);
    inserirInicio(ag, &m2);
    inserirFim(ag, &m3);
    inserirInicio(ag, &m4);
    inserirFim(ag, &m5);

    printf("--- Ordem Atual ---\n");
    exibirAgenda(ag);

    liberarAgenda(ag);
return 0;
}