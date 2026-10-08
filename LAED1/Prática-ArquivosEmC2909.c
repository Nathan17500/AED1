#include <stdio.h>
#include <stdlib.h>

int main () {
    FILE *arquivo;
    int c;
    char linha[100];

    arquivo = fopen("entrada.txt", "w");
    if (arquivo==NULL) {
        printf("Nao foi possivel criar o arquivo");
        exit(1);
    }
    else{
        printf("Arquivo criado.");
    }
    fputc('A', arquivo); // Escreve um caractere no arquivo
    fputs("Hello World!", arquivo); // Escreve uma string no arquivo
    fprintf(arquivo, "\nTeste com fprintf"); // Escreve uma string formatada no arquivo
    // c = fgetc(arquivo); // Lê um caractere por vez
    c = fgets(linha, 100, arquivo); // Lê uma linha por vez do arquivo
    printf("\n%c", c);
    fclose(arquivo);
    return 0;
}