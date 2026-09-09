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

void subir_altura(int *paltura, int altura) {
    *paltura=*paltura + altura;
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
    printf("PONTUAÇÃO: %d\n%p\n", *ppontuação, (void*) ppontuação);
}

void testeAltura (int *paltura){
    printf("ALTURA: %d\n%p\n\n", *paltura, (void*) paltura);    
}

int main() {
    int vidaInicial = 100, vidaAtual = vidaInicial, tesouroInicial = 0, tesouroAtual = tesouroInicial, pontuaçãoInicial = 0, pontuaçãoAtual = pontuaçãoInicial, plataformas[5] = {5, 10, 15, 20, 25}, númeroPlataforma = 0,alturaInicial = 0,  alturaAtual = alturaInicial;
    int *pvida = &vidaAtual, *ptesouro = &tesouroAtual, *ppontuação = &pontuaçãoAtual, *paltura = &alturaAtual;

        printf("Estado Inicial\n");
        verificaVidaAtual(pvida);
        testeTesouro(ptesouro);
        testePontuação(ppontuação);
        testeAltura(paltura);

        printf("Você acorda e se prepara para subir a montanha em busca do tesouro que se encontra no topo. Você sobe os primeiros 5 metros do percurso.\n");
        subir_altura(paltura, *(plataformas + númeroPlataforma));
        númeroPlataforma++;
        testeAltura(paltura);

        printf("Enquanto subia a trilha, um deslizamento de pedras te atinge, causando 30 de dano. Mesmo assim, você não desiste e continua sua jornada, subindo mais 10 metros.\n");
        aplicar_dano(pvida, 30);
        subir_altura(paltura, *(plataformas + númeroPlataforma));
        númeroPlataforma++;
        verificaVidaAtual(pvida);
        testeAltura(paltura);

        printf("Na metade do caminho, você é emboscado por outros que também buscam o tesouro. Você os derrota, recebendo 20 de dano e 100 pontos, continuando a subir 15 metros.\n");
        aplicar_dano(pvida, 20);
        *ppontuação = *ppontuação + 100;
        subir_altura(paltura, *(plataformas + númeroPlataforma));
        númeroPlataforma++;
        verificaVidaAtual(pvida);
        testePontuação(ppontuação);
        testeAltura(paltura);
        
        printf("Pensando em desistir devido aos ferimentos, você, por sorte, encontra um local para se recuperar. Você recupera 30 de vida e sobe mais 20 metros.\n");
        restaurar_vida(pvida, 30);
        subir_altura(paltura, *(plataformas + númeroPlataforma));
        númeroPlataforma++;
        verificaVidaAtual(pvida);
        testeAltura(paltura);

        printf("Após descansar, você adentra uma caverna no topo da montranha, encontrando o tesouro que tanto buscava, duplicando seus pontos e encerrando o dia.\n");
        *ptesouro=1;
        subir_altura(paltura, *(plataformas + númeroPlataforma));
        testeAltura(paltura);
        // O deslocamento respeita o tipo do ponteiro, já que cada tipo de dado tem um tamanho distinto, ele irá pular a quantidade de bytes correspondente ao tipo do ponteiro, como mostrado a seguir, ele pulará 4 bytes, o tamanho de um inteiro.
        for (int númeroPlataforma = 0; númeroPlataforma < 5; númeroPlataforma++) {
            *ppontuação = *ppontuação + *(plataformas + númeroPlataforma);
            printf("%d\n%d\n%p\n", *(plataformas + númeroPlataforma), númeroPlataforma, (void*) (plataformas + númeroPlataforma));
        }
        aplicar_pontuação_dupla(ppontuação);
        testePontuação(ppontuação);

        printf("\nInício do dia\nVIDA: %d\n", vidaInicial);
        if (tesouroInicial==0) {
            printf("TESOURO: INATIVO\n");
        } else {
            printf("TESOURO: ATIVO\n");
        }
        printf("PONTUAÇÃO: %d\nALTURA: %d\n", pontuaçãoInicial, alturaInicial);

        printf("\nFim do dia\n");
        verificaVidaAtual(pvida);
        testeTesouro(ptesouro);
        testePontuação(ppontuação);
        testeAltura(paltura);

    return 0;
}