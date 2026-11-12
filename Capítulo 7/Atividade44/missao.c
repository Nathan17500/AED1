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

int inserirFim (Agenda *ag, const Missao *m) {
    if (ag == NULL || m == NULL) {
        return 0;
    }

    No *novo = malloc (sizeof(No));
    if (novo == NULL) {
        return 0;
    }

    novo->dado = *m;
    novo->proximo =NULL;

    if (ag->inicio == NULL) {
        ag->inicio = novo;
    } else {
        No *ultimo = ag->inicio;
        while (ultimo->proximo != NULL) {
            ultimo = ultimo->proximo;
        }
        ultimo->proximo = novo;
    }

    return 1;
}

void exibirAgenda (const Agenda *ag) {
    if (ag == NULL) return;

    if (ag->inicio == NULL) {
        printf("Nenhuma missão cadastrada.\n");
        return;
    }

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

static No *buscarNo (const Agenda *ag, int id) {
    No *p = ag->inicio;

    while (p != NULL && p->dado.id != id) {
        p = p->proximo;
    }

    return p;
}

int missaoExiste (const Agenda *ag, int id) {
    if (ag == NULL) return;
    return buscarNo(ag, id) != NULL;
}

Resultado_Operacao inserirApos (Agenda *ag, int id_referencia, const Missao *m) {
    if (ag == NULL || m == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    No *ref = buscarNo (ag, id_referencia);
    if (ref == NULL) {
        return MISSAO_NAO_ENCONTRADA;
    }

    No *novo = malloc (sizeof(No));
    if (novo == NULL) {
        return ERRO_ALOCACAO;
    } 

    novo->dado = *m;
    novo->proximo = ref->proximo;
    ref->proximo = novo;

    return OPERACAO_OK;
}