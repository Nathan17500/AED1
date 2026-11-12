#include <stdio.h>
#include <string.h>
#include "missao.h"

Missao criarMissao (int id, char *titulo, int prioridade) {
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