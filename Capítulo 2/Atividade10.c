#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void inicializarCelulas(char *matriz[10][10]) {
    for (int linha = 0; linha < 10; linha++) {
        for (int coluna = 0; coluna < 10; coluna++) {
            matriz[linha][coluna] = NULL;
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

int main () {
    char *matriz[10][10];
    char **nome_jogadores, **senha_jogadores, **apelido_jogadores, **destino_jogadores;
    char nome[30], senha[30], senha_confirmação[30], apelido[30], equipe[30], separador[2] = {':'}, busca[30], jogador_selecionado[30], pergunta;
    int quantidade_jogadores;


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

    // O sizeof(*ponteiro) é melhor que usar diretamente o tipo usado, sizeof(* char) nesse caso, por permitir que o tipo da variável seja alterado sem que seja preciso alterar na chamada da função malloc também.
    nome_jogadores = malloc(quantidade_jogadores * sizeof(*nome_jogadores));
    if (nome_jogadores == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    senha_jogadores = malloc(quantidade_jogadores * sizeof(*senha_jogadores));
    if (senha_jogadores == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    apelido_jogadores = malloc(quantidade_jogadores * sizeof(*apelido_jogadores));
    if (apelido_jogadores == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    destino_jogadores = malloc(quantidade_jogadores * sizeof(*destino_jogadores));
    if (destino_jogadores == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    int linha[quantidade_jogadores], coluna[quantidade_jogadores];

    for (int i=0; i<quantidade_jogadores; i++) {
        nome_jogadores[i] = malloc (30 * sizeof(*nome_jogadores[i]));
        if (nome_jogadores[i] == NULL) {
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
            printf("Erro de alocação de memória");
            return 1;
        }

        printf("Informe o apelido do jogador %d: \n", i+1);
        fgets(apelido, sizeof(apelido), stdin);
        apelido[strcspn(apelido, "\n")] = '\0';
        strcpy(apelido_jogadores[i], apelido);

        senha_jogadores[i] = malloc (30 * sizeof(*senha_jogadores[i]));
        if (senha_jogadores[i] == NULL) {
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
    }
    for (int i = 0;i < quantidade_jogadores; i++) {
        destino_jogadores[i] = malloc (130 * sizeof(*destino_jogadores[i]));
        if (destino_jogadores[i] == NULL) {
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

    for (int i = 0; i < quantidade_jogadores; i++) {
        free(nome_jogadores[i]);
        free(senha_jogadores[i]);
        free(apelido_jogadores[i]);
        free(destino_jogadores[i]);
    }
    free(nome_jogadores);
    free(senha_jogadores);
    free(apelido_jogadores);
    free(destino_jogadores);

    nome_jogadores = NULL;
    senha_jogadores = NULL;
    apelido_jogadores = NULL;
    destino_jogadores = NULL;
}