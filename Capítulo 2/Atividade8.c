#include <stdio.h>
#include <string.h>

int main () {
    char nome_jogadores[3][30], senha_jogadores[3][30], apelido_jogadores[3][30], destino_jogadores[3][30];
    char nome[30], senha[30], senha_confirmação[30], apelido[30], equipe[200], separador[2] = {':'}, busca[30];

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
    printf("\nInforme um nome para buscar o jogador:\n");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';
    int encontrado = 0;
    for (int i = 0;i < 3; i++) {
        if (strcmp (busca, nome_jogadores[i]) == 0) {
            printf("%s\n", destino_jogadores[i]);
            encontrado = 1;
            break;
        }
    }
    if (encontrado != 1) {
        printf("Jogador não encontrado.");
    }
}