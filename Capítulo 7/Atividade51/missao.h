#ifndef MISSAO_H
#define MISSAO_H

#define TAMANHO_TITULO 30
#define PRIORIDADE_MIN 1
#define PRIORIDADE_MAX 5
#define LOTE_MAX       10

typedef enum {
    OPERACAO_OK,
    MISSAO_NAO_ENCONTRADA,
    ERRO_ALOCACAO,
    ARGUMENTO_INVALIDO,
    LISTA_VAZIA,
    SEM_SUCESSOR,
    SEM_ANTECESSOR,
    DADOS_INVALIDOS,
    LOTE_INVALIDO,
    ERRO_ABERTURA,
    ERRO_ESCRITA,
    ID_DUPLICADO,
    AGENDA_NAO_VAZIA,
    ARQUIVO_CORROMPIDO,
    ERRO_LEITURA
} Resultado_Operacao;

typedef struct {
    int id;
    char titulo[TAMANHO_TITULO];
    int prioridade;
    int concluida;
} Missao;

typedef struct Agenda Agenda;

/** Cria uma missão pendente. O título é truncado em TAMANHO_TITULO - 1 caracteres.
 *  Não valida a prioridade; a agenda valida ao inserir. */
Missao criarMissao(int id, const char *titulo, int prioridade);

/** Imprime uma missão em uma linha. */
void exibirMissao(const Missao *m);

/** Cria uma agenda vazia. Retorna NULL se faltar memória. */
Agenda *criarAgenda(void);

/** Libera todas as missões e a própria agenda. Aceita NULL. */
void liberarAgenda(Agenda *ag);

/** Insere uma cópia da missão no início. Retorna 1 se inseriu e 0 se os
 *  argumentos são NULL, os dados são inválidos, o id já existe ou falta memória. */
int inserirInicio(Agenda *ag, const Missao *m);

/** Insere uma cópia da missão no fim. Mesmos retornos de inserirInicio. */
int inserirFim(Agenda *ag, const Missao *m);

/** Insere a missão logo após a de id_referencia.
 *  Retorna ARGUMENTO_INVALIDO, DADOS_INVALIDOS, ID_DUPLICADO,
 *  MISSAO_NAO_ENCONTRADA (referência), ERRO_ALOCACAO ou OPERACAO_OK. */
Resultado_Operacao inserirApos(Agenda *ag, int id_referencia, const Missao *m);

/** Insere a missão logo antes da de id_referencia. Mesmos retornos de inserirApos. */
Resultado_Operacao inserirAntes(Agenda *ag, int id_referencia, const Missao *m);

/** Remove a primeira missão e a copia em *removida, se não for NULL.
 *  Retorna ARGUMENTO_INVALIDO, LISTA_VAZIA ou OPERACAO_OK. */
Resultado_Operacao removerInicio(Agenda *ag, Missao *removida);

/** Remove a última missão e a copia em *removida, se não for NULL.
 *  Retorna ARGUMENTO_INVALIDO, LISTA_VAZIA ou OPERACAO_OK. */
Resultado_Operacao removerFim(Agenda *ag, Missao *removida);

/** Remove a missão de id informado e a copia em *removida, se não for NULL.
 *  Retorna ARGUMENTO_INVALIDO, LISTA_VAZIA, MISSAO_NAO_ENCONTRADA ou OPERACAO_OK. */
Resultado_Operacao removerPorId(Agenda *ag, int id, Missao *removida);

/** Remove a missão seguinte à de id_referencia.
 *  Retorna ARGUMENTO_INVALIDO, LISTA_VAZIA, MISSAO_NAO_ENCONTRADA,
 *  SEM_SUCESSOR ou OPERACAO_OK. */
Resultado_Operacao removerApos(Agenda *ag, int id_referencia, Missao *removida);

/** Remove a missão anterior à de id_referencia.
 *  Retorna ARGUMENTO_INVALIDO, LISTA_VAZIA, MISSAO_NAO_ENCONTRADA,
 *  SEM_ANTECESSOR ou OPERACAO_OK. */
Resultado_Operacao removerAntes(Agenda *ag, int id_referencia, Missao *removida);

/** Imprime as missões na ordem da agenda. */
void exibirAgenda(const Agenda *ag);

/** Retorna 1 se existe missão com o id informado e 0 caso contrário. */
int missaoExiste(const Agenda *ag, int id);

/** Retorna a quantidade de missões. */
int contarMissoes(const Agenda *ag);

/** Retorna a quantidade de missões não concluídas. */
int contarPendentes(const Agenda *ag);

/** Remove todas as missões, mantendo a agenda utilizável. Retorna quantas foram removidas. */
int limparAgenda(Agenda *ag);

/** Substitui título, prioridade e situação da missão de id informado; o id não muda.
 *  Retorna ARGUMENTO_INVALIDO, DADOS_INVALIDOS, MISSAO_NAO_ENCONTRADA ou OPERACAO_OK. */
Resultado_Operacao alterarMissao(Agenda *ag, int id, const Missao *novos);

/** Acrescenta ao fim de 1 a LOTE_MAX missões, tudo ou nada.
 *  Retorna ARGUMENTO_INVALIDO, LOTE_INVALIDO (quantidade), DADOS_INVALIDOS,
 *  ID_DUPLICADO (na agenda ou dentro do lote), ERRO_ALOCACAO ou OPERACAO_OK. */
Resultado_Operacao importarLote(Agenda *ag, const Missao *lote, int quantidade);

/** Ordena por prioridade decrescente, desempatando por id crescente.
 *  Retorna ARGUMENTO_INVALIDO ou OPERACAO_OK. */
Resultado_Operacao ordenarAgenda(Agenda *ag);

/** Grava a agenda em formato binário, sobrescrevendo o arquivo: um int com o
 *  total seguido dos registros na ordem da lista.
 *  Retorna ARGUMENTO_INVALIDO, ERRO_ABERTURA, ERRO_ESCRITA ou OPERACAO_OK. */
Resultado_Operacao salvarAgenda(const Agenda *ag, const char *caminho);

/** Reconstrói a agenda a partir de um arquivo gerado por salvarAgenda, na mesma ordem.
 *  A carga é recusada com AGENDA_NAO_VAZIA se a agenda já tiver missões, o que impede
 *  duplicação; use limparAgenda antes para recarregar.
 *  Arquivo inexistente não é erro: retorna OPERACAO_OK e a agenda continua vazia.
 *  Cada registro é validado e o arquivo é lido por inteiro antes de alterar a agenda,
 *  então em qualquer erro a agenda permanece vazia.
 *  Retorna ARGUMENTO_INVALIDO, AGENDA_NAO_VAZIA, ERRO_ABERTURA (existe mas não abre),
 *  ERRO_LEITURA (falha de E/S durante a leitura),
 *  ARQUIVO_CORROMPIDO (cabeçalho ausente ou negativo, registros faltando ou sobrando),
 *  DADOS_INVALIDOS, ID_DUPLICADO (repetido no arquivo), ERRO_ALOCACAO ou OPERACAO_OK. */
Resultado_Operacao carregarAgenda(Agenda *ag, const char *caminho);

#endif
