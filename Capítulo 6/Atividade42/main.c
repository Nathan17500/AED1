#include <stdio.h>
#include "missao.h"

int main () {
    Agenda *ag = criarAgenda();
    if (ag == NULL) {
        printf("Falha ao criar a agenda.\n");
        return 1;
    }

    Missao a = criarMissao(1, "Resgatar o prefeito", 5);
    Missao b = criarMissao(2, "Colher flores", 1);

    inserirInicio(ag, &a);
    inserirInicio(ag, &b);

    exibirAgenda(ag);

    liberarAgenda(ag);
return 0;
}