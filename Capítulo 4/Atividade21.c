#include <stdio.h>

typedef struct Partida
{ 
    int id;
    char jogador[16];
    int pontuacao;
} Partida;

int salvarPartidas (const char *arquivo, const Partida *partidas, int n) {
    FILE *arq = fopen(arquivo, "w");
    if (arq == NULL) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        fprintf(arq, "%d %s %d\n", partidas[i].id, partidas[i].jogador, partidas[i].pontuacao);
    }
    fclose (arq);
    return 1;
}

int carregarPartidas (const char *arquivo, Partida *partidas, int max) {
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL) {
        return 0;
    }

    int n = 0; 

    while (n < max && fscanf(arq, "%d %s %d", &partidas[n].id, &partidas[n].jogador, &partidas[n].pontuacao) == 3) {
        n++;
    }

    fclose(arq);
    return n;
}

void mostrarPartidas (const Partida *partidas, int n) {
    for (int i = 0; i < n; i++) {
        printf("Partida %d\tID: %d | NOME: %s | PONTUACAO: %d\n", i+1 , partidas[i].id, partidas[i].jogador, partidas[i].pontuacao);
    }    
}

int main () {
    int quantidade_jogadores;

    printf("Informe quantos jogadores serão registrados:");
    scanf("%d", &quantidade_jogadores);

    Partida partidas[quantidade_jogadores];

    for (int i = 0; i < quantidade_jogadores; i++) {
        printf("Informe os dados (ID, NOME, PONTUACAO) do jogador %d:", i+1);
        scanf("%d %s %d", &partidas[i].id, &partidas[i].jogador, &partidas[i].pontuacao);
    }

    if (!salvarPartidas("registro.txt", partidas, quantidade_jogadores)) {
        printf("Erro ao gravar partida");
        return 1;
    }

    Partida lidas[quantidade_jogadores];

    int total_partidas = carregarPartidas("registro.txt", lidas, quantidade_jogadores);
    if (total_partidas == 0) {
        printf("Nenhuma dado foi carregada.");
        return 1;
    }

    mostrarPartidas(lidas, total_partidas);
    return 0;
}