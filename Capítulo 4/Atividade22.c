#include <stdio.h>

typedef struct Partida
{ 
    int id;
    char jogador[16];
    int pontuacao;
} Partida;

int salvarPartidas (const char *arquivo, const Partida *partidas, int n) {
    FILE *arq = fopen(arquivo, "a");
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

    while (n < max && fscanf(arq, "%d %s %d", &partidas[n].id, partidas[n].jogador, &partidas[n].pontuacao) == 3) {
        n++;
    }

    fclose(arq);
    return n;
}

int mostrarPartidas (const char *arquivo) {
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL) {
        printf("Registro vazio.\n");
        return 0;
    }

    Partida p;
    int n = 0;
    int r;

    while ((r = fscanf(arq, "%d %s %d", &p.id, p.jogador, &p.pontuacao)) == 3) {
        n++;
        printf("Partida %d\tID: %d | NOME: %s | PONTUACAO: %d\n", n , p.id, p.jogador, p.pontuacao);
    }
    
    int resultado = n;

    if (r == EOF) {
        if (ferror(arq)) {
            printf("Erro na leitura.\n");
            resultado = -1;
        } else if (n == 0) {
            printf("Registro vazio.\n");
        } else {
            printf("%d registros lidos.\n", n);
        }
    } else {
        printf("Dado errado.\n");
        resultado = -1;
    }

    fclose(arq);
    return resultado;
}

int main () {
    int quantidade_partidas;

    do {
        printf("Informe quantas partidas serão registradas:");
        scanf("%d", &quantidade_partidas);
    } while (quantidade_partidas <= 0);

    Partida partidas[quantidade_partidas];

    for (int i = 0; i < quantidade_partidas; i++) {
        printf("Informe os dados (ID, NOME, PONTUACAO) da partida %d:", i+1);
        scanf("%d %s %d", &partidas[i].id, partidas[i].jogador, &partidas[i].pontuacao);
    }

    if (!salvarPartidas("registro.txt", partidas, quantidade_partidas)) {
        printf("Erro ao gravar partidas");
        return 1;
    }

    printf ("\nRegistro\n");
    mostrarPartidas("registro.txt");
    return 0;
}