#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "missao.h"

typedef struct No{
    Missao dado;
    struct No *proximo;
} No;

struct Agenda {
    No *inicio;
};

Missao criarMissao (int id, const char *titulo, int prioridade) {
    Missao m;
    m.id = id;
    strncpy (m.titulo, titulo, TAMANHO_TITULO - 1);
    m.titulo[TAMANHO_TITULO - 1] = '\0';
    m.prioridade = prioridade;
    m.concluida = 0;
    return m;
}

void exibirMissao (const Missao *m) {
    printf("[%d] %s | prioridade: %d | %s\n", m->id, m->titulo, m->prioridade, m->concluida ? "concluida" : "pendente");
}

Agenda *criarAgenda (void) {
    Agenda *ag = malloc (sizeof(Agenda));
    if (ag == NULL) {
        return NULL;
    }

    ag->inicio = NULL;
    return ag;
}

int inserirInicio (Agenda *ag, const Missao *m) {
    if (ag == NULL || m == NULL) {
        return 0;
    }

    No *novo = malloc (sizeof(No));
    if (novo == NULL) {
        return 0;
    }

    novo->dado = *m;
    novo->proximo = ag->inicio;
    ag->inicio = novo;

    return 1;
}

void exibirAgenda (const Agenda *ag) {
    if (ag == NULL) return;

    for (const No *p = ag->inicio; p != NULL; p = p->proximo) {
        exibirMissao(&p->dado);
    }
}

void liberarAgenda (Agenda *ag) {
    if (ag == NULL) return;

    No *p = ag->inicio;
    while (p != NULL) {
        No *proximo = p->proximo;
        free (p);
        p = proximo;
    }

    free (ag);
}