#include <stdio.h>
#include "missao.h"

static const char *veredito (int obtido, int esperado) {
    return obtido == esperado ? "OK" : "ERRO";
}

static void conferir (const Agenda *ag, int total_esperado, int pendentes_esperados) {
    int total = contarMissoes(ag);
    int pendentes = contarPendentes(ag);

    printf("Total: %d (esperado %d) %s | Pendentes: %d (esperado %d) %s\n",
           total, total_esperado, veredito(total, total_esperado),
           pendentes, pendentes_esperados, veredito(pendentes, pendentes_esperados));
    exibirAgenda(ag);
}

static void mostrarImportacao (Resultado_Operacao r, int quantidade) {
    switch (r) {
        case OPERACAO_OK:
            printf("Lote de %d missoes importado.\n", quantidade);
            break;
        case LOTE_INVALIDO:
            printf("Quantidade de lote invalida (use de 1 a %d).\n", LOTE_MAX);
            break;
        case DADOS_INVALIDOS:
            printf("Lote contem dados invalidos. Nada foi importado.\n");
            break;
        case ERRO_ALOCACAO:
            printf("Falta de memoria. Nada foi importado.\n");
            break;
        default:
            printf("Argumento invalido.\n");
            break;
    }
}

static void importar (Agenda *ag, const Missao *lote, int quantidade) {
    printf("Antes:  %d missoes (%d pendentes)\n", contarMissoes(ag), contarPendentes(ag));
    mostrarImportacao(importarLote(ag, lote, quantidade), quantidade);
    printf("Depois: %d missoes (%d pendentes)\n", contarMissoes(ag), contarPendentes(ag));
}

int main (void) {
    Agenda *ag = criarAgenda();
    if (ag == NULL) {
        printf("Falha ao criar a agenda.\n");
        return 1;
    }

    printf("--- Agenda vazia ---\n");
    conferir(ag, 0, 0);

    Missao m1 = criarMissao(1, "Resgatar o prefeito", 5);
    Missao m2 = criarMissao(2, "Colher flores", 1);
    m2.concluida = 1;

    inserirFim(ag, &m1);
    inserirFim(ag, &m2);

    printf("\n--- Duas missoes (uma concluida) ---\n");
    conferir(ag, 2, 1);

    Missao lote[4] = {
        criarMissao(10, "Reforcar o muro", 4),
        criarMissao(11, "Treinar arqueiros", 3),
        criarMissao(12, "Preparar a colheita", 2),
        criarMissao(13, "Enviar mensageiro", 5)
    };

    printf("\n--- Importar lote de 4 ---\n");
    importar(ag, lote, 4);
    conferir(ag, 6, 5);

    printf("\n--- Quantidade zero ---\n");
    importar(ag, lote, 0);

    printf("\n--- Quantidade acima do limite ---\n");
    importar(ag, lote, LOTE_MAX + 1);       /* rejeitado antes de ler o vetor */

    printf("\n--- Vetor nulo ---\n");
    importar(ag, NULL, 4);

    Missao ruim[3] = {
        criarMissao(20, "Valida A", 2),
        criarMissao(21, "Valida B", 3),
        criarMissao(22, "Prioridade ruim", 9)
    };

    printf("\n--- Lote com item invalido ---\n");
    importar(ag, ruim, 3);
    conferir(ag, 6, 5);

    liberarAgenda(ag);
    return 0;
}