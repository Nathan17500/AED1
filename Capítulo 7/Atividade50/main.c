#include <stdio.h>
#include "missao.h"

static const char *veredito (int obtido, int esperado) {
    return obtido == esperado ? "OK" : "DIVERGENTE";
}

static void conferir (const Agenda *ag, int total_esperado, int pendentes_esperado) {
    int total = contarMissoes(ag);
    int pendentes = contarPendentes(ag);

    exibirAgenda(ag);
    printf("Total: %d (esperado %d) %s | Pendentes: %d (esperado %d) %s\n",
           total, total_esperado, veredito(total, total_esperado),
           pendentes, pendentes_esperado, veredito(pendentes, pendentes_esperado));
}

static void mostrarOrdenacao (Resultado_Operacao r) {
    if (r == OPERACAO_OK) {
        printf("Agenda ordenada.\n");
    } else {
        printf("Argumento invalido.\n");
    }
}

static void mostrarSalvar (Resultado_Operacao r, const char *caminho, const Agenda *ag) {
    switch (r) {
        case OPERACAO_OK:
            printf("Agenda salva em %s (%d missoes).\n", caminho, contarMissoes(ag));
            break;
        case ERRO_ABERTURA:
            printf("Nao foi possivel abrir '%s' para escrita.\n", caminho);
            break;
        case ERRO_ESCRITA:
            printf("Falha ao gravar em '%s'.\n", caminho);
            break;
        default:
            printf("Argumento invalido.\n");
            break;
    }
}

static void conferirArquivo (const char *caminho, int esperado) {
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) {
        printf("Nao foi possivel abrir %s para conferir.\n", caminho);
        return;
    }

    int total = 0;
    if (fread(&total, sizeof(total), 1, arq) != 1) {
        printf("Arquivo sem cabecalho.\n");
        fclose(arq);
        return;
    }

    printf("Arquivo: %d registros (esperado %d) %s\n", total, esperado, veredito(total, esperado));

    Missao m;
    int lidos = 0;
    while (lidos < total && fread(&m, sizeof(Missao), 1, arq) == 1) {
        exibirMissao(&m);
        lidos++;
    }

    if (lidos != total) {
        printf("ERRO: lidos %d de %d registros.\n", lidos, total);
    }

    fclose(arq);
}

int main (void) {
    Agenda *ag = criarAgenda();
    if (ag == NULL) {
        printf("Falha ao criar a agenda.\n");
        return 1;
    }

    printf("--- Agenda vazia ---\n");
    mostrarOrdenacao(ordenarAgenda(ag));
    conferir(ag, 0, 0);
    mostrarSalvar(salvarAgenda(ag, "agenda_vazia.bin"), "agenda_vazia.bin", ag);
    conferirArquivo("agenda_vazia.bin", 0);

    Missao m1 = criarMissao(1, "Resgatar o prefeito", 5);
    Missao m2 = criarMissao(2, "Colher flores", 1);
    Missao m3 = criarMissao(3, "Pescar", 2);
    Missao m4 = criarMissao(4, "Defender a cidade", 4);
    Missao m5 = criarMissao(5, "Consertar a ponte", 3);
    Missao m6 = criarMissao(6, "Treinar guardas", 5);
    Missao m7 = criarMissao(7, "Vigiar a muralha", 2);
    m2.concluida = 1;

    /* ordem de entrada propositalmente embaralhada, com empates de prioridade */
    inserirFim(ag, &m3);
    inserirFim(ag, &m6);
    inserirFim(ag, &m2);
    inserirFim(ag, &m5);
    inserirFim(ag, &m1);
    inserirFim(ag, &m7);
    inserirFim(ag, &m4);

    printf("\n--- Antes de ordenar ---\n");
    conferir(ag, 7, 6);

    printf("\n--- Ordenar (prioridade decrescente, empate por id) ---\n");
    mostrarOrdenacao(ordenarAgenda(ag));
    conferir(ag, 7, 6);

    printf("\n--- Salvar ---\n");
    mostrarSalvar(salvarAgenda(ag, "agenda.bin"), "agenda.bin", ag);
    conferirArquivo("agenda.bin", 7);

    printf("\n--- Salvar em caminho invalido ---\n");
    mostrarSalvar(salvarAgenda(ag, "pasta_inexistente/agenda.bin"),
                  "pasta_inexistente/agenda.bin", ag);
    conferir(ag, 7, 6);

    printf("\n--- Falha de gravacao (Linux: /dev/full) ---\n");
    mostrarSalvar(salvarAgenda(ag, "/dev/full"), "/dev/full", ag);

    liberarAgenda(ag);
    return 0;
}