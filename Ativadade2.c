#include <stdio.h>

void aplicarDano(int *pvida, int dano) {
    *pvida=*pvida - dano;
}

void aplicarCura(int *pvida, int cura) {
    *pvida=*pvida + cura;
}

void testetesouro(int *ptesouro){
    if (*ptesouro == 0){
        printf("TESOURO: INATIVO\n\n");
    }else{
        printf("TESOURO: ATIVO\n\n");
    }
}

void verificaVidaAtual(int *pvida){
    printf("VIDA: %d\n", *pvida);
}

int main() {
    int vidaInicial = 100, vidaAtual = vidaInicial, tesouroInicial = 0, tesouroAtual = tesouroInicial;
    int *pvida = &vidaAtual, *ptesouro = &tesouroAtual;

    printf("Estado Inicial\n");
    verificaVidaAtual(pvida);
    testetesouro(ptesouro);
    printf("%p\n%p\n", (void*)pvida, (void*)ptesouro);
    printf("Enquanto se aventurava em busca de tesouro, você cai em um buraco. Você recebe 30 de dano.\n");
    aplicarDano(pvida, 30);
    verificaVidaAtual(pvida);
    printf("Lá dentro, você se depara com um local ideal para descansar. Você recupera sua vida!\n");
    aplicarCura(pvida, 20);
    verificaVidaAtual(pvida);
    printf("Após descansar, você adentra mais a caverna, encontrando o tesouro que tanto procurava e encerrando o dia.\n\n");
    *ptesouro=1;
    printf("Início do dia\nVIDA: %d\n", vidaInicial);
    if (tesouroInicial==0) {
        printf("TESOURO: INATIVO\n");
    } else {
        printf("TESOURO: ATIVO\n");
    }
    printf("\nFim do dia\n");
    verificaVidaAtual(pvida);
    testetesouro(ptesouro);

    return 0;
}