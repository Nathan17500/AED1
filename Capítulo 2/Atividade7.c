#include <stdio.h>
#include <string.h>

int main () {
    char *nome_jogadores[3], *senha_jogadores[3];
    char nome[30], senha[30];

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
        printf("Informe a senha do jogador %d: \n");
        fgets(senha, sizeof(senha), stdin);
        senha[strcspn(nome, "\n")] = '\0';

    }

}