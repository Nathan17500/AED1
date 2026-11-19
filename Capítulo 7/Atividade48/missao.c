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

static No *buscarNo (const Agenda *ag, int id) {
    No *p = ag->inicio;

    while (p != NULL && p->dado.id != id) {
        p = p->proximo;
    }

    return p;
}

int missaoExiste (const Agenda *ag, int id) {
    if (ag == NULL) return 0;
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

Resultado_Operacao inserirAntes (Agenda *ag, int id_referencia, const Missao *m) {
    if (ag == NULL || m == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    No *anterior = NULL;
    No *atual = ag->inicio;

    while (atual != NULL && atual->dado.id != id_referencia) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return MISSAO_NAO_ENCONTRADA;
    }

    No *novo = malloc (sizeof(No));
    if (novo == NULL) {
        return ERRO_ALOCACAO;
    } 

    novo->dado = *m;
    novo->proximo = atual;

    if (anterior == NULL) {
        ag->inicio = novo;
    } else {
        anterior->proximo = novo;
    }

    return OPERACAO_OK;
}

Resultado_Operacao removerInicio (Agenda *ag, Missao *removida) {
    if (ag == NULL) {
        return ARGUMENTO_INVALIDO;
    } 

    if (ag->inicio == NULL) {
        return LISTA_VAZIA;
    }

    No *alvo = ag->inicio;

    if (removida != NULL) {
        *removida = alvo->dado;
    }

    ag->inicio = alvo->proximo;
    free(alvo);

    return OPERACAO_OK;
}   

Resultado_Operacao removerFim (Agenda *ag, Missao *removida) {
    if (ag == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    if (ag->inicio == NULL) {
        return LISTA_VAZIA;
    }

    No *anterior = NULL;
    No *atual = ag->inicio;

    while (atual->proximo != NULL) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (removida != NULL) {
        *removida = atual->dado;
    }

    if (anterior == NULL) {
        ag->inicio = NULL;
    } else {
        anterior->proximo = NULL;
    }

    free(atual);

    return OPERACAO_OK;
}

Resultado_Operacao removerPorId (Agenda *ag, int id, Missao *removida) {
    if (ag == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    if (ag->inicio == NULL) {
        return LISTA_VAZIA;
    }

    No *anterior = NULL;
    No *atual = ag->inicio;

    while (atual != NULL && atual->dado.id != id) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return MISSAO_NAO_ENCONTRADA;
    }

    if (removida != NULL) {
        *removida = atual->dado;
    }

    if (anterior == NULL) {
        ag->inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free (atual);

    return OPERACAO_OK;
}

int contarMissoes (const Agenda *ag) {
    if (ag == NULL) return 0;

    int total = 0;
    for (const No *p = ag->inicio; p != NULL; p = p->proximo) {
        total++;
    }

    return total;
}

Resultado_Operacao removerApos (Agenda *ag, int id_referencia, Missao *removida) {
    if (ag == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    if (ag->inicio == NULL) {
        return LISTA_VAZIA;
    }

    No *ref = buscarNo(ag, id_referencia);
    if (ref == NULL) {
        return MISSAO_NAO_ENCONTRADA;
    }

    No *alvo = ref->proximo;
    if (alvo == NULL) {
        return SEM_SUCESSOR;
    }

    if (removida != NULL) {
        *removida = alvo->dado;
    }

    ref->proximo = alvo->proximo;
    free(alvo);

    return OPERACAO_OK;
}

Resultado_Operacao removerAntes (Agenda *ag, int id_referencia, Missao *removida) {
    if (ag == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    if (ag->inicio == NULL) {
        return LISTA_VAZIA;
    }

    No *ante_anterior = NULL;
    No *anterior = NULL;
    No *atual = ag->inicio;

    while (atual != NULL && atual->dado.id != id_referencia) {
        ante_anterior = anterior;
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return MISSAO_NAO_ENCONTRADA;
    }

    if (anterior == NULL) {
        return SEM_ANTECESSOR;
    }

    if (removida != NULL) {
        *removida = anterior->dado;
    }

    if (ante_anterior == NULL) {
        ag->inicio = atual;
    } else {
        ante_anterior->proximo = atual;
    }

    free(anterior);

    return OPERACAO_OK;
}

int limparAgenda (Agenda *ag) {
    if (ag == NULL) {
        return 0;
    }

    int liberados = 0;
    No *p = ag->inicio;

    while (p != NULL) {
        No *proximo = p->proximo;
        free(p);
        p = proximo;
        liberados++;
    }

    ag->inicio = NULL;

    return liberados;
}

void liberarAgenda (Agenda *ag) {
    if (ag == NULL) return;

    limparAgenda(ag);
    free(ag);
}

static int dadosValidos (const Missao *m) {
    /* o título precisa ter terminador dentro do vetor */
    if (memchr(m->titulo, '\0', TAMANHO_TITULO) == NULL) {
        return 0;
    }

    /* e não pode ser vazio */
    if (m->titulo[0] == '\0') {
        return 0;
    }

    if (m->prioridade < PRIORIDADE_MIN || m->prioridade > PRIORIDADE_MAX) {
        return 0;
    }

    if (m->concluida != 0 && m->concluida != 1) {
        return 0;
    }

    return 1;
}

Resultado_Operacao alterarMissao (Agenda *ag, int id, const Missao *novos) {
    if (ag == NULL || novos == NULL) {
        return ARGUMENTO_INVALIDO;
    }

    if (!dadosValidos(novos)) {
        return DADOS_INVALIDOS;
    }

    No *no = buscarNo(ag, id);
    if (no == NULL) {
        return MISSAO_NAO_ENCONTRADA;
    }

    strncpy(no->dado.titulo, novos->titulo, TAMANHO_TITULO - 1);
    no->dado.titulo[TAMANHO_TITULO - 1] = '\0';
    no->dado.prioridade = novos->prioridade;
    no->dado.concluida = novos->concluida;

    return OPERACAO_OK;
}