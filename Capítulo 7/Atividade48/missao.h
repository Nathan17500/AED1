#ifndef MISSAO_H
#define MISSAO_H

#define TAMANHO_TITULO 30
#define PRIORIDADE_MIN 1
#define PRIORIDADE_MAX 5

typedef enum {
    OPERACAO_OK,
    MISSAO_NAO_ENCONTRADA,
    ERRO_ALOCACAO,
    ARGUMENTO_INVALIDO,
    LISTA_VAZIA,
    SEM_SUCESSOR,
    SEM_ANTECESSOR,
    DADOS_INVALIDOS
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

Resultado_Operacao inserirAntes(Agenda *ag, int id_referencia, const Missao *m);
Resultado_Operacao removerInicio(Agenda *ag, Missao *removida);

Resultado_Operacao removerFim(Agenda *ag, Missao *removida);
Resultado_Operacao removerPorId(Agenda *ag, int id, Missao *removida);
int                contarMissoes(const Agenda *ag);

Resultado_Operacao removerApos(Agenda *ag, int id_referencia, Missao *removida);
Resultado_Operacao removerAntes(Agenda *ag, int id_referencia, Missao *removida);

int                limparAgenda(Agenda *ag);
Resultado_Operacao alterarMissao(Agenda *ag, int id, const Missao *novos);

#endif