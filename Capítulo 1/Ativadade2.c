#include <stdio.h>

void aplicar_pontuação_dupla (int *ppontuação) {
    *ppontuação=*ppontuação * 2;
}

void aplicar_dano(int *pvida, int dano) {
    *pvida=*pvida - dano;
}

void restaurar_vida(int *pvida, int cura) {
    *pvida=*pvida + cura;
}

void testeTesouro(int *ptesouro){
    if (*ptesouro == 0){
        printf("TESOURO: INATIVO\n%p\n", (void*) ptesouro);
    }else{
        printf("TESOURO: ATIVO\n%p\n", (void*) ptesouro);
    }
}

void verificaVidaAtual(int *pvida){
    printf("VIDA: %d\n%p\n", *pvida, (void*) pvida);
}

void testePontuação (int *ppontuação){
    printf("PONTUAÇÃO: %d\n%p\n\n", *ppontuação, (void*) ppontuação);
}

int main() {
    int vidaInicial = 100, vidaAtual = vidaInicial, tesouroInicial = 0, tesouroAtual = tesouroInicial, pontuaçãoInicial = 0, pontuaçãoAtual = pontuaçãoInicial;
    int *pvida = &vidaAtual, *ptesouro = &tesouroAtual, *ppontuação = &pontuaçãoAtual;

        printf("Estado Inicial\n");
        verificaVidaAtual(pvida);
        testeTesouro(ptesouro);
        testePontuação(ppontuação);

        printf("Enquanto se aventurava em busca de tesouro, você cai em um buraco. Você recebe 30 de dano.\n");
        aplicar_dano(pvida, 30);
        verificaVidaAtual(pvida);

        printf("Lá dentro, você se depara com um local ideal para descansar. Você recupera sua vida e ganha 100 pontos!\n");
        restaurar_vida(pvida, 20);
        *ppontuação = *ppontuação + 100;
        verificaVidaAtual(pvida);
        testePontuação(ppontuação);

        printf("Após descansar, você adentra mais a caverna, encontrando o tesouro que tanto procurava, duplicando seus pontos e encerrando o dia.\n\n");
        *ptesouro=1;
        aplicar_pontuação_dupla(ppontuação);

        printf("Início do dia\nVIDA: %d\n", vidaInicial);
        if (tesouroInicial==0) {
            printf("TESOURO: INATIVO\n");
        } else {
            printf("TESOURO: ATIVO\n");
        }
        printf("PONTUAÇÃO: %d\n", pontuaçãoInicial);

        printf("\nFim do dia\n");
        verificaVidaAtual(pvida);
        testeTesouro(ptesouro);
        testePontuação(ppontuação);

    return 0;
}