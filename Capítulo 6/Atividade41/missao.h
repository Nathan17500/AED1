#ifndef MISSAO_H
#define MISSAO_H

#define TAMANHO_TITULO 30

typedef struct {
    int id;
    char titulo[TAMANHO_TITULO];
    int prioridade;
    int concluida;
} Missao;

Missao criarMissao (int id, char *titulo, int prioridade);
void exibirMissao (const Missao *m);

#endif