#include <stdio.h>
#include <string.h>

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
    char nome_jogadores[3][30], senha_jogadores[3][30], apelido_jogadores[3][30], destino_jogadores[3][30];
    char nome[30], senha[30], senha_confirmação[30], apelido[30], equipe[100], separador[2] = {':'}, busca[30], jogador_selecionado[30], pergunta;
    int linha[3], coluna[3];

    printf("Informe o nome da equipe:\n");
    fgets(equipe, sizeof(equipe), stdin);
    equipe[strcspn(equipe, "\n")] = '\0';
    strcat(equipe, separador);
    for (int i=0; i<3; i++) {
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

        printf("Informe o apelido do jogador %d: \n", i+1);
        fgets(apelido, sizeof(apelido), stdin);
        apelido[strcspn(apelido, "\n")] = '\0';
        strcpy(apelido_jogadores[i], apelido);

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
    for (int i = 0;i < 3; i++) {
        strcpy(destino_jogadores[i], equipe);
        strcat(destino_jogadores[i], apelido_jogadores[i]);
        printf("%s\n", destino_jogadores[i]);
    }
    inicializarCelulas(matriz);
    exibirMapa (matriz);
    printf("O mapa acima está vazio, defina as posições dos jogadores.\n");
    for (int i = 0;i < 3; i++) {
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
                for (i = 0;i < 3; i++) {
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
}