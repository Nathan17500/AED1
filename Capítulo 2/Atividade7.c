#include <stdio.h>
#include <string.h>

int main () {
    char *nome_jogadores[3], *senha_jogadores[3], *apelido_jogadores[3];
    char nome[30], senha[30], senha_confirmação[30], apelido[30];

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
        nome_jogadores[i] = nome;

        printf("Informe o apelido do jogador %d: \n", i+1);
        fgets(apelido, sizeof(apelido), stdin);
        apelido[strcspn(apelido, "\n")] = '\0';
        apelido_jogadores[i] = apelido;

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
            senha_jogadores[i] = senha;
        } else {
            senha_jogadores[i] = senha;
        }
    }
}