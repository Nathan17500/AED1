#include <stdio.h>
#include <locale.h>

void etapa_padrão(int *mapa, int *petapa){
    printf("Você descansa um pouco antes de prosseguir para a próxima etapa. Você sobe %d metros.", mapa[*petapa]);
    mostrar_etapa(petapa);
}

void mostrar_etapa(int *petapa){
    printf("Etapa: %d", *petapa);
    *petapa = *petapa + 1;
}

void ler_mapa(int *mapa, int tamanho) {
    for (int i=0;i<tamanho;i++) {
        scanf("%d", mapa+i);
    }
}

void mostrar_mapa(const int *mapa, int tamanho){
    for (int i=0;i<tamanho;i++){
        printf("Etapa %d: %d metros.", i+1, *(mapa+i));
    }
}

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
    setlocale(LC_ALL, "Portuguese");
    int vidaInicial = 100, vidaAtual = vidaInicial, tesouroInicial = 0, tesouroAtual = tesouroInicial, pontuaçãoInicial = 0, pontuaçãoAtual = pontuaçãoInicial,alturaInicial = 0,  alturaAtual = alturaInicial, tamanho, etapaAtual=0;
    int *pvida = &vidaAtual, *ptesouro = &tesouroAtual, *ppontuação = &pontuaçãoAtual, *paltura = &alturaAtual, *petapa = &etapaAtual;

        printf("Estado Inicial\n");
        verificaVidaAtual(pvida);
        testeTesouro(ptesouro);
        testePontuação(ppontuação);

        printf("Você acorda e se prepara para subir a montanha em busca do tesouro que se encontra no topo. Informe quantas etapas você realizará durante o percurso.\n");
        scanf("%d", &tamanho);
        int mapa[tamanho];
        printf("Informe também quanto você deseja subir entre cada etapa.");
        ler_mapa (mapa, tamanho);
        mostrar_mapa (mapa, tamanho);

        if (etapaAtual == 0) {
            printf("Enquanto subia a trilha, um deslizamento de pedras te atinge, causando 30 de dano. Mesmo assim, você não desiste e continua sua jornada, subindo mais %d metros.\n", mapa[*petapa]);
            aplicar_dano(pvida, 30);
            verificaVidaAtual(pvida);
        } else {
            etapa_padrão(mapa, petapa);
        }

        if (etapaAtual == tamanho - 3) {
            printf("Na metade do caminho, você é emboscado por outros que também buscam o tesouro. Você os derrota, recebendo 20 de dano e 100 pontos, continuando a subir 15 metros.\n");
            aplicar_dano(pvida, 20);
            *ppontuação = *ppontuação + 100;
            verificaVidaAtual(pvida);
            testePontuação(ppontuação);
        } else {
            etapa_padrão(mapa, petapa);
        }

        if (etapaAtual == tamanho - 2) {
            printf("Pensando em desistir devido aos ferimentos, você, por sorte, encontra um local para se recuperar. Você recupera 30 de vida e sobe mais 20 metros.\n");
            restaurar_vida(pvida, 30);
            verificaVidaAtual(pvida);
        }  else {
            etapa_padrão(mapa, petapa);
        }
        // adicionar for para repetir as etapas até chegar no marco.
        if (etapaAtual == tamanho) {
            printf("Após descansar, você adentra uma caverna no topo da montranha, encontrando o tesouro que tanto buscava, duplicando seus pontos e encerrando o dia.\n");
            *ptesouro=1;
            aplicar_pontuação_dupla(ppontuação);
            testePontuação(ppontuação);
        } else {
            etapa_padrão(mapa, petapa);
        }

        printf("\nInício do dia\nVIDA: %d\n", vidaInicial);
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