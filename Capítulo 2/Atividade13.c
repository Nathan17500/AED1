#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INT 0
#define STRING 1
#define CHAR 2
#define LINHAS_MAX 200
#define COLUNAS_MAX 200

void encerrarCadastro (char **nome, char **senha, char **apelido, char **destino, int alocados) {
    for (int i = 0; i < alocados; i++) {
        free(nome[i]);
        nome[i] = NULL;
        free(senha[i]);
        senha[i] = NULL;
        free(apelido[i]);
        apelido[i] = NULL;
        if (destino[i] != NULL) {
            free(destino[i]);
            destino[i] = NULL;
        }
    }
    free(nome);
    nome = NULL;
    free(senha);
    senha = NULL;
    free(apelido);
    apelido = NULL;
    free(destino);
    destino = NULL;
}

void funçãoGenérica (void *ponteiro, int tipo) {
    if (ponteiro == NULL) {
        printf("Nulo\n");
        return;
    }
    switch (tipo) {
        case INT:
            printf("%d\n", *(int *)ponteiro);
            break;
        case STRING:
            printf("%s\n", (char *)ponteiro);
            break;
        case CHAR:
            printf("%c\n", *(char *)ponteiro);
            break;
        default:
            printf ("Tipo desconhecido.");
    }
}

void inicializarCelulas(char *matriz[10][10]) {
    for (int linha = 0; linha < 10; linha++) {
        for (int coluna = 0; coluna < 10; coluna++) {
            matriz[linha][coluna] = NULL;
        }
    }
}

int *alocarHistorico(int tamanho) {
    if (tamanho <= 0 || tamanho > 200) {
        return NULL;
    }
    int *historico = malloc (tamanho * sizeof(*historico));
    return historico;
}

void preencherHistorico(int *historico, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("Informe o resultado da partida %d:\n", i + 1);
        scanf("%d", &historico[i]);
    }
}

void exibirHistorico(int *historico, int tamanho) {
    printf("HISTORICO DE PARTIDAS:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Partida %d: %d\n", i + 1, historico[i]);
    }
}

float calcularMedia(int *historico, int tamanho) {
    int soma = 0;
    for (int i = 0; i < tamanho; i++) {
        soma += historico[i];
    }
    return (float)soma/tamanho;
}

void encontrarMaior(int *historico, int tamanho, int *maiorValor, int *posicao) {
    *maiorValor = historico[0];
    *posicao = 0;

    for (int i = 1; i < tamanho; i++) {
        if (historico[i] > *maiorValor) {
            *maiorValor = historico[i];
            *posicao = i;
        }
    }
}

void posicionarJogador(int *linha, int *coluna, char *matriz[10][10], char *apelido) {
    printf("Escolha uma posição para comecar, informando as coordenadas linha e coluna:");
    do {
        scanf("%d %d", linha, coluna);
        if (*linha < 0 || *linha > 9 || *coluna < 0 || *coluna > 9) {
            printf("Posição inválida. Informe novamente.\n");
        } else if (matriz[*linha][*coluna] != NULL) {
            printf("Posição ocupada. Informe novamente.\n");
        }
    } while (*linha < 0 || *linha > 9 || *coluna < 0 || *coluna > 9 || matriz[*linha][*coluna] != NULL);
    matriz[*linha][*coluna] = apelido;
}

void reposicionarJogador(int *linha, int *coluna, char *matriz[10][10], char *apelido) {
    int nlinha, ncoluna;

    do {
    printf("Informe a nova posição do jogador, informando as coordenadas linha e coluna:");
    scanf("%d %d", &nlinha, &ncoluna);
        if (nlinha < 0 || nlinha >= 10 || ncoluna < 0 || ncoluna >= 10) {
            printf("Posição inválida. Informe novamente.\n");
        } else if (matriz[nlinha][ncoluna] != NULL) {
            printf("Posição ocupada. Informe novamente.\n");
        } else {
            break;
        }
    } while (nlinha < 0 || nlinha > 9 || ncoluna < 0 || ncoluna > 9 || matriz[nlinha][ncoluna] != NULL);

    matriz[*linha][*coluna] = NULL;
    *linha = nlinha;
    *coluna = ncoluna;
    matriz[*linha][*coluna] = apelido;
}

void exibirMapa(char *matriz[10][10]) {
    printf("\nMAPA:\n\n");

    for (int mlinha = 0; mlinha < 10; mlinha++) {
        for (int mcoluna = 0; mcoluna < 10; mcoluna++) {
            if (matriz[mlinha][mcoluna] != NULL) {
                printf("%s\t", matriz[mlinha][mcoluna]);
            } else {
                printf("0\t");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int *alocarMatrizLinear(int linhas, int colunas) {
    if (linhas <= 0 || colunas <= 0) {
        return NULL;
    }

    if (linhas > LINHAS_MAX || colunas > COLUNAS_MAX) {
        return NULL;
    } 

    int totalEspaços = linhas * colunas;
    int *matriz = malloc(totalEspaços * sizeof(*matriz));
    return matriz;
}

void preencherMatrizLinear(int *matriz, int linhas, int colunas) {
    for (int linha = 0; linha < linhas; linha ++) {
        for (int coluna = 0; coluna < colunas; coluna ++) {
            printf("Informe o valor da posição [%d][%d]", linha, coluna);
            scanf("%d", &matriz[linha * colunas + coluna]);
        }
    }
}

void exibirMatrizLinear(int *matriz, int linhas, int colunas) {
    printf("\nMATRIZ LINEAR:\n");
    for (int linha = 0; linha < linhas; linha++) {
        for (int coluna = 0; coluna < colunas; coluna++) {
            printf("%d\t", matriz[linha * colunas + coluna]);
        }
        printf("\n");
    }
    printf("\n");
}

void liberarMatrizLinear(int *matriz) {
    free(matriz);
}

int **alocarMatrizPonteiros(int linhas, int colunas) {
    if (linhas <= 0 || colunas <= 0) {
        return NULL;
    }

    if (linhas > LINHAS_MAX || colunas > COLUNAS_MAX) {
        return NULL;
    } 

    int **matriz = malloc (linhas * sizeof(*matriz));
    if (matriz == NULL) {
        return NULL;
    }

    int linhasAlocadas = 0;
    for (int linha = 0; linha < linhas; linha++) {
        matriz[linha] = malloc(colunas * sizeof(*matriz[linha]));
        if (matriz[linha] == NULL) {
            for (int i = 0; i < linhasAlocadas; i++) {
                free(matriz[i]);
                matriz[i] = NULL;
            }
            free(matriz);
            return NULL;
        }
        linhasAlocadas = linha + 1;
    }
 
    return matriz;
}

void preencherMatrizPonteiros(int **matriz, int linhas, int colunas) {
    for (int linha = 0; linha < linhas; linha++) {
        for (int coluna = 0; coluna < colunas; coluna++) {
            printf("Informe o valor da posição [%d][%d]: ", linha, coluna);
            scanf("%d", &matriz[linha][coluna]);
        }
    }
}

void exibirMatrizPonteiros(int **matriz, int linhas, int colunas) {
    printf("\nMATRIZ PONTEIRO DE PONTEIROS (linhas independentes):\n");
    for (int linha = 0; linha < linhas; linha++) {
        for (int coluna = 0; coluna < colunas; coluna++) {
            printf("%d\t", matriz[linha][coluna]);
        }
        printf("\n");
    }
    printf("\n");
}

void liberarMatrizPonteiros(int **matriz, int linhas) {
    if (matriz == NULL) {
        return;
    }
    for (int linha = 0; linha < linhas; linha++) {
        free(matriz[linha]);
        matriz[linha] = NULL;
    }
    free(matriz);
}

void lerDimensoesMapa(int *linhas, int *colunas) {
    do {
        printf("Informe o numero de linhas do mapa dinamico (1 a %d):\n", LINHAS_MAX);
        scanf("%d", linhas);
        printf("Informe o numero de colunas do mapa dinamico (1 a %d):\n", COLUNAS_MAX);
        scanf("%d", colunas);
        if (*linhas <= 0 || *colunas <= 0) {
            printf("Dimensoes invalidas. Ambas devem ser maiores que zero.\n");
        } else if (*linhas > LINHAS_MAX || *colunas > COLUNAS_MAX) {
            printf("Dimensoes acima do limite maximo permitido.\n");
        }
    } while (*linhas <= 0 || *colunas <= 0 || *linhas > LINHAS_MAX || *colunas > COLUNAS_MAX);
}

int main () {
    char *matriz[10][10];
    char **nome_jogadores, **senha_jogadores, **apelido_jogadores, **destino_jogadores;
    char nome[30], senha[30], senha_confirmação[30], apelido[30], equipe[30], separador[2] = {':'}, busca[30], jogador_selecionado[30], pergunta;
    int quantidade_jogadores, maior_valor, posicao_maior, tamanho;
    float media;

    printf("Informe o nome da equipe:\n");
    fgets(equipe, sizeof(equipe), stdin);
    equipe[strcspn(equipe, "\n")] = '\0';
    strcat(equipe, separador);

    do {
        printf("Informe quantos jogadores a equipe terá:\n");
        scanf("%d", &quantidade_jogadores);
        if (quantidade_jogadores <= 0) {
            printf("Quantidade Inválida. Informe novamente\n");
        }
    } while (quantidade_jogadores <= 0);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    int alocados = 0;

    // O sizeof(*ponteiro) é melhor que usar diretamente o tipo usado, sizeof(* char) nesse caso, por permitir que o tipo da variável seja alterado sem que seja preciso alterar na chamada da função malloc também.
    nome_jogadores = malloc(quantidade_jogadores * sizeof(*nome_jogadores));
    if (nome_jogadores == NULL) {
        free(nome_jogadores);
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    senha_jogadores = malloc(quantidade_jogadores * sizeof(*senha_jogadores));
    if (senha_jogadores == NULL) {
        free(nome_jogadores);
        free(senha_jogadores);
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    apelido_jogadores = malloc(quantidade_jogadores * sizeof(*apelido_jogadores));
    if (apelido_jogadores == NULL) {
        free(nome_jogadores);
        free(senha_jogadores);
        free(apelido_jogadores);
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    destino_jogadores = malloc(quantidade_jogadores * sizeof(*destino_jogadores));
    if (destino_jogadores == NULL) {
        free(nome_jogadores);
        free(senha_jogadores);
        free(apelido_jogadores);
        free(destino_jogadores);
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    int linha[quantidade_jogadores], coluna[quantidade_jogadores];

    for (int i=0; i<quantidade_jogadores; i++) {
        nome_jogadores[i] = malloc (30 * sizeof(*nome_jogadores[i]));
        if (nome_jogadores[i] == NULL) {
            encerrarCadastro(nome_jogadores, senha_jogadores, apelido_jogadores, destino_jogadores, alocados);
            printf("Erro de alocação de memória");
            return 1;
        }

        printf("Informe o nome do jogador %d: \n", i+1);
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0';
        if (strlen(nome) == 0) {
            do {
                printf ("Nome inválido. Informe novamente:\n");
                fgets(nome,sizeof(nome), stdin);
            } while (strlen(nome) == 0);
        }
        strcpy(nome_jogadores[i], nome);

        apelido_jogadores[i] = malloc (30 * sizeof(*apelido_jogadores[i]));
        if (apelido_jogadores[i] == NULL) {
            encerrarCadastro(nome_jogadores, senha_jogadores, apelido_jogadores, destino_jogadores, alocados);
            printf("Erro de alocação de memória");
            return 1;
        }
        printf("Informe o apelido do jogador %d: \n", i+1);
        fgets(apelido, sizeof(apelido), stdin);
        apelido[strcspn(apelido, "\n")] = '\0';
        strcpy(apelido_jogadores[i], apelido);

        senha_jogadores[i] = malloc (30 * sizeof(*senha_jogadores[i]));
        if (senha_jogadores[i] == NULL) {
            encerrarCadastro(nome_jogadores, senha_jogadores, apelido_jogadores, destino_jogadores, alocados);
            printf("Erro de alocação de memória");
            return 1;
        }
        printf("Informe a senha do jogador %d: \n", i+1);
        fgets(senha, sizeof(senha), stdin);
        senha[strcspn(senha, "\n")] = '\0';
        if (strlen(senha) == 0) {
            do {
                printf ("Senha inválida. Informe novamente:\n");
                fgets(senha, sizeof(senha), stdin);
            } while (strlen(senha) == 0);
        }

        printf("Confirme sua senha:\n");
        fgets(senha_confirmação, sizeof(senha_confirmação), stdin);
        senha_confirmação[strcspn(senha_confirmação, "\n")] = '\0';
        if (strcmp (senha, senha_confirmação) != 0) {
            while (strcmp (senha, senha_confirmação) != 0) {
                printf("Senha errada. Informe novamente.\n");
                fgets(senha_confirmação,sizeof(senha_confirmação), stdin);
                senha_confirmação[strcspn(senha_confirmação, "\n")] = '\0';
            }
            strcpy(senha_jogadores[i], senha);
        } else {
            strcpy(senha_jogadores[i], senha);
        }

        alocados = i + 1;
    }

    for (int i = 0;i < quantidade_jogadores; i++) {
        destino_jogadores[i] = malloc (130 * sizeof(*destino_jogadores[i]));
        if (destino_jogadores[i] == NULL) {
            encerrarCadastro(nome_jogadores, senha_jogadores, apelido_jogadores, destino_jogadores, alocados);
            printf("Erro de alocação de memória");
            return 1;
        }
        strcpy(destino_jogadores[i], equipe);
        strcat(destino_jogadores[i], apelido_jogadores[i]);
        printf("%s\n", destino_jogadores[i]);
    }

    inicializarCelulas(matriz);
    exibirMapa (matriz);
    printf("O mapa acima está vazio, defina as posições dos jogadores.\n");
    for (int i = 0;i < quantidade_jogadores; i++) {
        posicionarJogador(&linha[i], &coluna[i], matriz, destino_jogadores[i]);
        exibirMapa(matriz);
    }
    do {
        printf("Você gostaria de reposicionar algum jogador? S/N\n");
        scanf(" %c", &pergunta);
        int c; 
        while ((c = getchar()) != '\n' && c != EOF);
        if (pergunta == 'S') {
            printf("Informe o nome do jogador:\n");
            fgets(busca, sizeof(busca), stdin);
            busca[strcspn(busca, "\n")] = '\0';
            int encontrado = 0;
            int i;
            do {
                for (i = 0;i < quantidade_jogadores; i++) {
                    if (strcmp (busca, nome_jogadores[i]) == 0) {
                        printf("%s\n", destino_jogadores[i]);
                        encontrado = 1;
                        strcpy (jogador_selecionado, destino_jogadores[i]);
                        break;
                    }
                }
                if (encontrado != 1) {
                    printf("Jogador não encontrado. Informe novamente");
                    fgets(busca, sizeof(busca), stdin);
                    busca[strcspn(busca, "\n")] = '\0';
                }
            } while (encontrado != 1);
            reposicionarJogador(&linha[i], &coluna[i], matriz, jogador_selecionado);
            exibirMapa(matriz);
        }
        if (pergunta == 'N') {
            break;
        }
    } while (pergunta == 'S');

    do {
        printf("Informe quantas partidas quer registrar?\n");
        scanf("%d", &tamanho);
    } while (tamanho <= 0);

    int *historico = alocarHistorico(tamanho);
    if (historico == NULL) {
        printf("Tamanho inválido ou memoria insuficiente.\n");
        return 1;
    }

    preencherHistorico(historico, tamanho);
    exibirHistorico(historico, tamanho);

    media = calcularMedia(historico, tamanho);
    printf("Média: %f\n", media);

    encontrarMaior(historico, tamanho, &maior_valor, &posicao_maior);
    printf("Maior pontuação: %d (partida %d)\n", maior_valor, posicao_maior + 1);

    int linhas_mapa, colunas_mapa;
    lerDimensoesMapa(&linhas_mapa, &colunas_mapa);
 
    int *mapaLinear = alocarMatrizLinear(linhas_mapa, colunas_mapa);
    if (mapaLinear == NULL) {
        printf("Erro ao alocar a matriz linear.\n");
    }
 
    int **mapaPonteiros = alocarMatrizPonteiros(linhas_mapa, colunas_mapa);
    if (mapaPonteiros == NULL) {
        printf("Erro ao alocar a matriz de ponteiros.\n");
    }
 
    if (mapaLinear != NULL && mapaPonteiros != NULL) {
        char opcaoMenu;
        do {
            printf("\nMENU\n");
            printf("1 - Preencher matriz linear\n");
            printf("2 - Exibir matriz linear\n");
            printf("3 - Preencher matriz de ponteiros\n");
            printf("4 - Exibir matriz de ponteiros\n");
            printf("5 - Sair do menu de mapas\n");
            printf("Escolha uma opcao: ");
            scanf(" %c", &opcaoMenu);
            int cMenu;
            while ((cMenu = getchar()) != '\n' && cMenu != EOF);
 
            switch (opcaoMenu) {
                case '1':
                    preencherMatrizLinear(mapaLinear, linhas_mapa, colunas_mapa);
                    break;
                case '2':
                    exibirMatrizLinear(mapaLinear, linhas_mapa, colunas_mapa);
                    break;
                case '3':
                    preencherMatrizPonteiros(mapaPonteiros, linhas_mapa, colunas_mapa);
                    break;
                case '4':
                    exibirMatrizPonteiros(mapaPonteiros, linhas_mapa, colunas_mapa);
                    break;
                case '5':
                    printf("Saindo do menu de mapas.\n");
                    break;
                default:
                    printf("Opcao invalida.\n");
            }
        } while (opcaoMenu != '5');
    }
 
    liberarMatrizLinear(mapaLinear);
    liberarMatrizPonteiros(mapaPonteiros, linhas_mapa);

    encerrarCadastro( nome_jogadores, senha_jogadores, apelido_jogadores, destino_jogadores, alocados);
    free(historico);
    historico = NULL;
    return 0;
}