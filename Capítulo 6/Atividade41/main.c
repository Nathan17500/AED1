#include <stdio.h>
#include "missao.h"

int main () {

Missao a = criarMissao(1, "Resgatar o prefeito", 5);
Missao b = criarMissao(2, "Colher flores", 1);

exibirMissao(&a);
exibirMissao(&b);

return 0;
}