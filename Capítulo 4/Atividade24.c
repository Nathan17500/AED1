#include <stdio.h>
#include <string.h>

typedef struct Partida
{ 
    int id;
    char jogador[16];
    int pontuacao;
} Partida;

int processarTemporada (const char *arquivo_origem, const char *arquivo_destino) {
    FILE *origem = fopen(arquivo_origem, "r");
    if (origem == NULL) {
        printf("Falha na leitura de %s.\n", arquivo_origem);
        return -1;
    }
    
    FILE *destino = fopen(arquivo_destino, "w");
    if (destino == NULL) {
        printf("Falha ao criar o destino %s\n", arquivo_destino);
        fclose(origem);
        return -1;
    }

    Partida p;
    int processados = 0;
    int r;

    // Bônus: Caso uma partida possua uma pontuação maior ou igual a 50, recebe um bônus de 10%.
    while ((r = fscanf(origem, "%d %15s %d", &p.id, p.jogador, &p.pontuacao)) == 3) {
        int pontuacao_final = p.pontuacao;
        if (pontuacao_final >= 50) {
            pontuacao_final = p.pontuacao + p.pontuacao / 10;
        }

        if (fprintf(destino, "%d %s %d\n", p.id, p.jogador, pontuacao_final) < 0) {
            printf("Erro de gravação.\n");
            fclose(origem);
            fclose(destino);
            return -1;
        } 
        processados++;
    }

    int resultado = processados;

    if (r == EOF) {
        if (ferror(origem)) {
            printf("Erro de leitura na origem.\n");
            resultado = -1;
        }
    } else {
        printf("Dado com erro na origem após %d registros válidos.\n", processados);
        resultado = -1;
    }

    fclose(origem);
    fclose(destino);

    printf("%d registros processados.\n", processados);
    return resultado;
}

int salvarPartidasTexto (const char *arquivo, const Partida *partidas, int n) {
    FILE *arq = fopen(arquivo, "a");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", arquivo);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        fprintf(arq, "%d %s %d\n", partidas[i].id, partidas[i].jogador, partidas[i].pontuacao);
    }
    fclose (arq);
    return 1;
}

int salvarPartidasBinario(const char *arquivo, const Partida *partida) {
    FILE *arq = fopen(arquivo, "wb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", arquivo);
        return 0;
    }

    size_t escritos = fwrite(partida, sizeof(Partida), 1, arq);
    if (escritos != 1) {
        printf("Erro ao gravar o arquivo %s.\n", arquivo);
        fclose(arq);
        return 0;
    }

    fclose(arq);
    return 1;
}

int carregarPartidasTexto (const char *arquivo, Partida *partidas, int max) {
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

int carregarPartidasBinario(const char *arquivo, Partida *partida) {
    FILE *arq = fopen(arquivo, "rb");
    if (arq == NULL) {
        printf("Falha ao abrir o arquivo %s.\n", arquivo);
        return -1;
    }

    size_t lidos = fread(partida, 1, sizeof(Partida), arq);

    if (lidos == sizeof(Partida)) {
        fclose(arq);
        return 1;
    }

    if (ferror(arq)) {
        printf("Erro de leitura no arquivo binario.\n");
        fclose(arq);
        return -1;
    }

    if (lidos == 0) {
        printf("Arquivo vazio.\n");
        fclose(arq);
        return 0;
    }

    printf("Registro incompleto: %zu de %zu bytes.\n", lidos, sizeof(Partida));
    fclose(arq);
    return -1;
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

int compararPartidas(const Partida *a, const Partida *b) {
    return a->id == b->id && strcmp(a->jogador, b->jogador) == 0 && a->pontuacao == b->pontuacao;
}

int main () {
    int quantidade_partidas;

    do {
        printf("Informe quantas partidas serão registradas:");
        scanf("%d", &quantidade_partidas);
    } while (quantidade_partidas <= 0);

    Partida partidas[quantidade_partidas];
    memset(partidas, 0, sizeof(partidas));

    for (int i = 0; i < quantidade_partidas; i++) {
        printf("Informe os dados (ID, NOME, PONTUACAO) da partida %d:", i+1);
        scanf("%d %s %d", &partidas[i].id, partidas[i].jogador, &partidas[i].pontuacao);
    }

    if (!salvarPartidasTexto("registro.txt", partidas, quantidade_partidas)) {
        printf("Erro ao gravar partidas.\n");
        return 1;
    }

    printf ("\nRegistro\n");
    mostrarPartidas("registro.txt");

    printf("\nProcessando Temporada...\n");
    processarTemporada("registro.txt", "relatorio.txt");

    printf("\nTeste do arquivo binario...\n");

    Partida original;
    memset(&original, 0, sizeof(Partida));
    original = partidas[0];   /* usa a primeira partida digitada */

    if (!salvarPartidasBinario("partida.bin", &original)) {
        return 1;
    }

    Partida restaurada;
    memset(&restaurada, 0, sizeof(Partida));

    int status = carregarPartidasBinario("partida.bin", &restaurada);
    if (status == 1) {
        printf("Restaurada -> ID: %d | NOME: %s | PONTUACAO: %d\n",
            restaurada.id, restaurada.jogador, restaurada.pontuacao);

        if (compararPartidas(&original, &restaurada)) {
            printf("Registro restaurado igual ao original.\n");
        } else {
            printf("Registro restaurado diferente do original.\n");
        }
    }
    return 0;
}