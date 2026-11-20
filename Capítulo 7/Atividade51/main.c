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

static void mostrarCarga (Resultado_Operacao r, const char *caminho, const Agenda *ag) {
    switch (r) {
        case OPERACAO_OK:
            printf("Carga de %s concluida (%d missoes na agenda).\n", caminho, contarMissoes(ag));
            break;
        case AGENDA_NAO_VAZIA:
            printf("Carga recusada: a agenda ja possui missoes.\n");
            break;
        case ERRO_ABERTURA:
            printf("Nao foi possivel abrir '%s' para leitura.\n", caminho);
            break;
        case ERRO_LEITURA:
            printf("Falha ao ler '%s'.\n", caminho);
            break;
        case ARQUIVO_CORROMPIDO:
            printf("Arquivo '%s' corrompido ou incompleto.\n", caminho);
            break;
        case DADOS_INVALIDOS:
            printf("Arquivo '%s' contem missao invalida.\n", caminho);
            break;
        case ID_DUPLICADO:
            printf("Arquivo '%s' contem id repetido.\n", caminho);
            break;
        case ERRO_ALOCACAO:
            printf("Memoria insuficiente ao carregar '%s'.\n", caminho);
            break;
        default:
            printf("Argumento invalido.\n");
            break;
    }
}

static void gravarBruto (const char *caminho, int total, const Missao *regs, int n) {
    FILE *arq = fopen(caminho, "wb");
    if (arq == NULL) {
        printf("Nao foi possivel criar %s.\n", caminho);
        return;
    }

    fwrite(&total, sizeof(total), 1, arq);
    if (n > 0) {
        fwrite(regs, sizeof(Missao), (size_t)n, arq);
    }

    fclose(arq);
}

static int arquivosIguais (const char *a, const char *b) {
    FILE *fa = fopen(a, "rb");
    FILE *fb = fopen(b, "rb");
    int iguais = (fa != NULL && fb != NULL);

    while (iguais) {
        int ca = fgetc(fa);
        int cb = fgetc(fb);
        if (ca != cb) {
            iguais = 0;
        } else if (ca == EOF) {
            break;
        }
    }

    if (fa != NULL) fclose(fa);
    if (fb != NULL) fclose(fb);
    return iguais;
}

static void testarCarga (const char *caminho) {
    Agenda *ag = criarAgenda();
    if (ag == NULL) {
        printf("Falha ao criar a agenda.\n");
        return;
    }

    mostrarCarga(carregarAgenda(ag, caminho), caminho, ag);
    conferir(ag, 0, 0);
    liberarAgenda(ag);
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

    printf("\n--- Carregar arquivo inexistente ---\n");
    Agenda *nova = criarAgenda();
    if (nova == NULL) {
        printf("Falha ao criar a agenda.\n");
        liberarAgenda(ag);
        return 1;
    }
    mostrarCarga(carregarAgenda(nova, "nao_existe.bin"), "nao_existe.bin", nova);
    conferir(nova, 0, 0);

    printf("\n--- Carregar agenda vazia salva ---\n");
    mostrarCarga(carregarAgenda(nova, "agenda_vazia.bin"), "agenda_vazia.bin", nova);
    conferir(nova, 0, 0);

    printf("\n--- Carregar agenda.bin (ordem preservada) ---\n");
    mostrarCarga(carregarAgenda(nova, "agenda.bin"), "agenda.bin", nova);
    conferir(nova, 7, 6);
    mostrarSalvar(salvarAgenda(nova, "agenda_recarregada.bin"), "agenda_recarregada.bin", nova);
    printf("Ordem identica a original: %s\n",
           veredito(arquivosIguais("agenda.bin", "agenda_recarregada.bin"), 1));

    printf("\n--- Carregar de novo sem limpar (recusado) ---\n");
    mostrarCarga(carregarAgenda(nova, "agenda.bin"), "agenda.bin", nova);
    conferir(nova, 7, 6);

    printf("\n--- Limpar e carregar de novo ---\n");
    printf("Removidas: %d\n", limparAgenda(nova));
    mostrarCarga(carregarAgenda(nova, "agenda.bin"), "agenda.bin", nova);
    conferir(nova, 7, 6);

    printf("\n--- Argumentos invalidos ---\n");
    mostrarCarga(carregarAgenda(NULL, "agenda.bin"), "agenda.bin", NULL);
    mostrarCarga(carregarAgenda(nova, NULL), "(nulo)", nova);

    Missao ruim = criarMissao(9, "Prioridade fora da faixa", 9);
    Missao dois[2]  = { m1, m2 };
    Missao ruins[2] = { m1, ruim };
    Missao iguais[2] = { m1, m1 };

    gravarBruto("truncado.bin", 3, dois, 2);
    gravarBruto("excedente.bin", 1, dois, 2);
    gravarBruto("invalido.bin", 2, ruins, 2);
    gravarBruto("repetido.bin", 2, iguais, 2);
    gravarBruto("negativo.bin", -1, NULL, 0);
    FILE *vazio = fopen("sem_cabecalho.bin", "wb");
    if (vazio != NULL) {
        fclose(vazio);
    }

    printf("\n--- Arquivo truncado ---\n");
    testarCarga("truncado.bin");
    printf("\n--- Arquivo com registros sobrando ---\n");
    testarCarga("excedente.bin");
    printf("\n--- Arquivo com missao invalida ---\n");
    testarCarga("invalido.bin");
    printf("\n--- Arquivo com id repetido ---\n");
    testarCarga("repetido.bin");
    printf("\n--- Arquivo com total negativo ---\n");
    testarCarga("negativo.bin");
    printf("\n--- Arquivo sem cabecalho ---\n");
    testarCarga("sem_cabecalho.bin");
    printf("\n--- Caminho que nao e arquivo (diretorio) ---\n");
    testarCarga(".");

    liberarAgenda(nova);
    liberarAgenda(ag);
    return 0;
}