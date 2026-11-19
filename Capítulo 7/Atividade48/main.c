#include <stdio.h>
#include "missao.h"

static void mostrarAlteracao (int id, Resultado_Operacao r) {
    switch (r) {
        case OPERACAO_OK:
            printf("Missao %d alterada.\n", id);
            break;
        case MISSAO_NAO_ENCONTRADA:
            printf("Missao %d nao foi encontrada.\n", id);
            break;
        case DADOS_INVALIDOS:
            printf("Dados invalidos. Nada foi alterado.\n");
            break;
        default:
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

    inserirFim(ag, &m1);
    inserirFim(ag, &m2);
    inserirFim(ag, &m3);

    printf("--- Lista inicial ---\n");
    conferir(ag, 3);

    Missao corrigida = criarMissao(99, "Colher ervas raras", 3);
    corrigida.concluida = 1;

    printf("\n--- Alterar id 2 (dados validos) ---\n");
    mostrarAlteracao(2, alterarMissao(ag, 2, &corrigida));
    conferir(ag, 3);
    printf("Existe missao 99? %s\n", missaoExiste(ag, 99) ? "sim" : "nao");

    printf("\n--- Alterar id 77 (ausente) ---\n");
    mostrarAlteracao(77, alterarMissao(ag, 77, &corrigida));
    conferir(ag, 3);

    Missao prioridade_ruim = criarMissao(3, "Pescar truta", 9);
    Missao titulo_vazio = criarMissao(3, "", 2);

    printf("\n--- Alterar id 3 (prioridade invalida) ---\n");
    mostrarAlteracao(3, alterarMissao(ag, 3, &prioridade_ruim));
    printf("\n--- Alterar id 3 (titulo vazio) ---\n");
    mostrarAlteracao(3, alterarMissao(ag, 3, &titulo_vazio));
    conferir(ag, 3);

    printf("\n--- Limpar a campanha ---\n");
    printf("Nos liberados: %d\n", limparAgenda(ag));
    conferir(ag, 0);

    printf("\n--- Limpar de novo (ja vazia) ---\n");
    printf("Nos liberados: %d\n", limparAgenda(ag));
    conferir(ag, 0);

    printf("\n--- Reusar a agenda apos limpar ---\n");
    inserirFim(ag, &m1);
    conferir(ag, 1);

    printf("\n--- Limpar agenda inexistente ---\n");
    printf("Nos liberados: %d\n", limparAgenda(NULL));

    liberarAgenda(ag);
    return 0;
}