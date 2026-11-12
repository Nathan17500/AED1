#ifndef MISSAO_H
#define MISSAO_H

#define TAMANHO_TITULO 30

typedef enum {
    OPERACAO_OK,
    MISSAO_NAO_ENCONTRADA,
    ERRO_ALOCACAO,
    ARGUMENTO_INVALIDO
} Resultado_Operacao;

typedef struct {
    int id;
    char titulo[TAMANHO_TITULO];
    int prioridade;
    int concluida;
} Missao;

typedef struct Agenda Agenda;

Missao  criarMissao (int id, const char *titulo, int prioridade);
void    exibirMissao (const Missao *m);

Agenda *criarAgenda(void);
int     inserirInicio(Agenda *ag, const Missao *m);
int     inserirFim(Agenda *ag, const Missao *m);
void    exibirAgenda(const Agenda *ag);
void    liberarAgenda(Agenda *ag);

int                missaoExiste(const Agenda *ag, int id);
Resultado_Operacao inserirApos(Agenda *ag, int id_referencia, const Missao *m);

#endif