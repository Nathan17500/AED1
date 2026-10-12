#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PARTIDAS 100000

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

int salvarTemporada(const char *arquivo, const Partida *partidas, int total) {
    FILE *arq = fopen(arquivo, "wb");
    if (arq == NULL) {
        printf("Falha ao criar '%s'.\n", arquivo);
        return 0;
    }

    if (fwrite(&total, sizeof(int), 1, arq) != 1) {
        printf("Erro ao gravar a quantidade de partidas.\n");
        fclose(arq);
        return 0;
    }

    if (total > 0) {
        size_t escritos = fwrite(partidas, sizeof(Partida), total, arq);
        if (escritos != (size_t)total) {
            printf("Erro de gravacao: %zu de %d registro(s) escritos.\n", escritos, total);
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);
    return 1;
}

int carregarTemporada(const char *arquivo, Partida **partidas, int *total) {
    FILE *arq = fopen(arquivo, "rb");
    if (arq == NULL) {
        printf("Falha ao abrir '%s'.\n", arquivo);
        return 0;
    }

    if (fseek(arq, 0, SEEK_END) != 0) {
        printf("Erro ao medir o arquivo.\n");
        fclose(arq);
        return 0;
    }
    long tamanho = ftell(arq);
    rewind(arq);

    if (tamanho == 0) {
        printf("Arquivo vazio.\n");
        fclose(arq);
        return 0;
    }

    int n;
    if (tamanho < (long)sizeof(int) || fread(&n, sizeof(int), 1, arq) != 1) {
        printf("Cabecalho incompleto ou erro de leitura.\n");
        fclose(arq);
        return 0;
    }

    if (n < 0 || n > MAX_PARTIDAS) {
        printf("Quantidade invalida no arquivo: %d.\n", n);
        fclose(arq);
        return 0;
    }

    long esperado = (long)sizeof(int) + (long)n * (long)sizeof(Partida);
    if (tamanho != esperado) {
        printf("Arquivo corrompido ou incompleto: %ld bytes, esperado %ld.\n", tamanho, esperado);
        fclose(arq);
        return 0;
    }

    Partida *novo = NULL;
    if (n > 0) {
        novo = malloc((size_t)n * sizeof(Partida));
        if (novo == NULL) {
            printf("Memoria insuficiente para %d partida(s).\n", n);
            fclose(arq);
            return 0;
        }

        size_t lidos = fread(novo, sizeof(Partida), n, arq);
        if (lidos != (size_t)n) {
            printf("Leitura incompleta: %zu de %d registro(s).\n", lidos, n);
            free(novo);
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);

    free(*partidas);
    *partidas = novo;
    *total = n;
    return 1;
}

int cadastrarPartida(Partida **partidas, int *total) {
    Partida nova;
    memset(&nova, 0, sizeof(Partida));

    printf("Informe os dados (ID NOME PONTUACAO): ");
    if (scanf("%d %15s %d", &nova.id, nova.jogador, &nova.pontuacao) != 3) {
        printf("Entrada invalida.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }

    Partida *tmp = realloc(*partidas, (size_t)(*total + 1) * sizeof(Partida));
    if (tmp == NULL) {
        printf("Memoria insuficiente.\n");
        return 0;
    }

    *partidas = tmp;
    (*partidas)[*total] = nova;
    (*total)++;
    return 1;
}

void listarPartidas(const Partida *partidas, int total) {
    if (total == 0) {
        printf("Nenhuma partida cadastrada.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("Partida %d\tID: %d | NOME: %s | PONTUACAO: %d\n",
               i + 1, partidas[i].id, partidas[i].jogador, partidas[i].pontuacao);
    }
    printf("%d partida(s).\n", total);
}

int main () {
    Partida *partidas = NULL;
    int total = 0;
    int opcao;

    do {
        printf("\n1 - Cadastrar partida\n2 - Listar partidas\n3 - Salvar temporada\n4 - Carregar temporada\n0 - Sair\nOpcao: ");

        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                cadastrarPartida(&partidas, &total);
                break;
            case 2:
                listarPartidas(partidas, total);
                break;
            case 3:
                if (salvarTemporada("temporada.bin", partidas, total)) {
                    printf("%d partida(s) salva(s).\n", total);
                }
                break;
            case 4:
                if (carregarTemporada("temporada.bin", &partidas, &total)) {
                    printf("%d partida(s) carregada(s).\n", total);
                }
                break;
            case 0:
                printf("Encerrando.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    free(partidas);
    return 0;
}
