#include <stdio.h>
#include <locale.h>

void etapa_padrão(int *mapa, int *petapa){
    printf("Você descansa um pouco antes de prosseguir para a próxima etapa. Você sobo %d metros.\n", mapa[*petapa]);
}

void cursor(int *petapa, int mapa[], int *pmapa, int *ppontuação, int *pitemValor){
    // Enquanto o acesso por índice permite acessar um valor de um vetor sem que ele mantenha sua progressão, o incremento de ponteiro permite que o ponteiro avance nos elementos de um vetor, mantendo sua progressão.
    pmapa = mapa + *petapa;
    printf("ETAPA %d: %d metros\n", *petapa+1, *pmapa);
    *petapa = *petapa + 1;
    if (*pitemValor == 30) {
        *ppontuação = *ppontuação + *pmapa + *pitemValor;
        *pitemValor = 0;
    } else {
        *ppontuação = *ppontuação + *pmapa;
    }
}

void ler_mapa(int *mapa, int tamanho) {
    for (int i=0;i<tamanho;i++) {
        scanf("%d", mapa+i);
    }
}

void mostrar_mapa(const int *mapa, int tamanho){
    for (int i=0;i<tamanho;i++){
        printf("Etapa %d: %d metros.\n", i+1, *(mapa+i));
    }
}

void aplicar_pontuação_dupla (int *ppontuação) {
    *ppontuação=*ppontuação * 2;
}

void aplicar_dano(int *pvida, int *armadura, int dano) {
    *pvida=*pvida - (dano - *armadura);
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
    printf("PONTUAÇÃO: %d\n%p\n\n", *ppontuação, (void*) ppontuação);
}

void testeAltura (int *paltura){
    printf("ALTURA: %d\n%p\n\n", *paltura, (void*) paltura);    
}

int escolherInventario(int *inventario[3]){
    int i;
    printf("\nINVENTÁRIO:\n");
    for (int c = 0; c < 3; c++) {
        if (c == 0) {
            printf("1.Poção de Cura: Recupera %d de Vida.\n", *(*(inventario + c)));
        } else if (c == 1) {
            printf("2.Capacete: Fornece %d de armadura.\n", *(*(inventario + c)));
        } else if (c == 2) {
            printf("3.Corda: Permite que você suba %d a mais.\n", *(*(inventario + c)));
        }
    }
    printf("4. Não use um item.\n\nEscolha um número para indicar o que vai usar:");
    scanf("%d", &i);
    if (i-1 <= 3 && i-1 >= 0) {
        return i-1;
    } else {
        do {
            printf("Valor inválido. Insira novamente.");
            scanf ("%d", &i);
        } while (i-1 > 3 || i-1 < 0);
        return i-1;
    }
}  

void usarItem(int *inventario[3], int i, int *itemValor) {
    if (i > 3||i < 0) {
        do {
            printf("Valor inválido. Informe novamente.\n");
            scanf("%d", &i);
        } while (i > 3 || i < 0);
    } 
    if (i == 3) {
        *itemValor = 0;
    } else if (i == 2) {
        printf("Corda usada, %d metros serão subidos a mais na próxima etapa.\n", *(*(inventario + i)));
        *itemValor = *inventario[i];
        *inventario[i] = 0;
    } else if (i == 1) {
        printf("Capacete equipado, %d de armadura adicionada.\n", *(*(inventario + i)));
        *itemValor = *inventario[i];
        *inventario[i] = 0;
    } else if (i == 0) {
        printf("Poção de Cura usada, %d de vida recuperada.\n", *(*(inventario + i)));
        *itemValor = *inventario[i];
        *inventario[i] = 0;
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int vidaInicial = 100, vidaAtual = vidaInicial, tesouroInicial = 0, tesouroAtual = tesouroInicial, pontuaçãoInicial = 0, pontuaçãoAtual = pontuaçãoInicial,alturaInicial = 0,  alturaAtual = alturaInicial, tamanho, etapaAtual = 0, pocao = 20, capacete = 15, corda = 30, itemValor = 0, item, armadura = 0;
    int *pvida = &vidaAtual, *ptesouro = &tesouroAtual, *ppontuação = &pontuaçãoAtual, *paltura = &alturaAtual, *petapa = &etapaAtual, *inventario[3] = {&pocao, &capacete, &corda}, *pitemValor = &itemValor, *parmadura = &armadura;

        printf("Estado Inicial\n");
        verificaVidaAtual(pvida);
        testeTesouro(ptesouro);
        testePontuação(ppontuação);

        printf("Você acorda e se prepara para subir a montanha em busca do tesouro que se encontra no topo. Informe quantas etapas você realizará durante o percurso.\n");
        scanf("%d", &tamanho);
        if (tamanho == 0 || tamanho < 0) {
            printf("Você decidiu não andar hoje.");
            return 0;
        }
        int mapa[tamanho];
        int *pmapa = mapa;
        printf("Informe também quanto você deseja subir entre cada etapa.");
        ler_mapa (mapa, tamanho);
        mostrar_mapa (mapa, tamanho);
        item = escolherInventario (inventario);
        if (item != 3) {
            usarItem (inventario, item, pitemValor);
            switch (item) {
                case 0:
                    *pvida += *pitemValor;
                    break;
                case 1:
                    *parmadura += *pitemValor;
                    break;
            }
        }
        printf("\n");

        printf("Enquanto subia a trilha, um deslizamento de pedras te atinge, causando 30 de dano. Mesmo assim, você não desiste e continua sua jornada, subindo mais %d metros.\n", mapa[*petapa]);
        aplicar_dano(pvida, parmadura,30);
        verificaVidaAtual(pvida);
        etapaAtual= etapaAtual+1;
        printf("ETAPA %d: %d metros\n", *petapa, mapa[*petapa-1]);
        pontuaçãoAtual = mapa[0];
        testePontuação(ppontuação);
        item = escolherInventario (inventario);
        if (item != 3) {
            usarItem (inventario, item, pitemValor);
            switch (item) {
                case 0:
                    *pvida += *pitemValor;
                    break;
                case 1:
                    *parmadura += *pitemValor;
                    break;
            }
        }

        while (etapaAtual <= tamanho - 3) {
            if (etapaAtual == tamanho - 3) {
                printf("Na metade do caminho, você é emboscado por outros que também buscam o tesouro. Você os derrota, recebendo 20 de dano e 100 pontos, continuando a subir %d metros.\n", mapa[*petapa]);
                aplicar_dano(pvida, parmadura,20);
                *ppontuação = *ppontuação + 100;
                verificaVidaAtual(pvida);
                cursor(petapa, mapa, pmapa, ppontuação, pitemValor);
                testePontuação(ppontuação);
                item = escolherInventario (inventario);
                if (item != 3) {
                    usarItem (inventario, item, pitemValor);
                    switch (item) {
                        case 0:
                            *pvida += *pitemValor;
                            break;
                        case 1:
                            *parmadura += *pitemValor;
                            break;
                    }
                }
            } else {
                etapa_padrão(mapa, petapa);
                cursor(petapa, mapa, pmapa, ppontuação, pitemValor);
                testePontuação(ppontuação);
                item = escolherInventario (inventario);
                if (item != 3) {
                    usarItem (inventario, item, pitemValor);
                    switch (item) {
                        case 0:
                            *pvida += *pitemValor;
                            break;
                        case 1:
                            *parmadura += *pitemValor;
                            break;
                    }
                }
            }
        }

        while (etapaAtual <= tamanho - 2){
            if (etapaAtual == tamanho - 2) {
                printf("Pensando em desistir devido aos ferimentos, você, por sorte, encontra um local para se recuperar. Você recupera 30 de vida e sobe mais %d metros.\n", mapa[*petapa]);
                restaurar_vida(pvida, 30);
                verificaVidaAtual(pvida);
                cursor(petapa, mapa, pmapa, ppontuação, pitemValor);
                testePontuação(ppontuação);
                item = escolherInventario (inventario);
                if (item != 3) {
                    usarItem (inventario, item, pitemValor);
                    switch (item) {
                        case 0:
                            *pvida += *pitemValor;
                            break;
                        case 1:
                            *parmadura += *pitemValor;
                            break;
                    }
                }
            }  else {
                etapa_padrão(mapa, petapa);
                cursor(petapa, mapa, pmapa, ppontuação, pitemValor);
                testePontuação(ppontuação);
                item = escolherInventario (inventario);
                if (item != 3) {
                    usarItem (inventario, item, pitemValor);
                    switch (item) {
                        case 0:
                            *pvida += *pitemValor;
                            break;
                        case 1:
                            *parmadura += *pitemValor;
                            break;
                    }
                }
            }
        }

        while (etapaAtual <= tamanho-1){
            if (etapaAtual == tamanho-1) {
                printf("Após descansar, você adentra uma caverna no topo da montranha, encontrando o tesouro que tanto buscava, duplicando seus pontos e encerrando o dia.");
                *ptesouro=1;
                aplicar_pontuação_dupla(ppontuação);
                testePontuação(ppontuação);
                cursor(petapa, mapa, pmapa, ppontuação, pitemValor);
                testePontuação(ppontuação);
                item = escolherInventario (inventario);
                if (item != 3) {
                    usarItem (inventario, item, pitemValor);
                    switch (item) {
                        case 0:
                            *pvida += *pitemValor;
                            break;
                        case 1:
                            *parmadura += *pitemValor;
                            break;
                    }
                }
            } else {
                etapa_padrão(mapa, petapa);
                cursor(petapa, mapa, pmapa, ppontuação, pitemValor);
                testePontuação(ppontuação);
                item = escolherInventario (inventario);
                if (item != 3) {
                    usarItem (inventario, item, pitemValor);
                    switch (item) {
                        case 0:
                            *pvida += *pitemValor;
                            break;
                        case 1:
                            *parmadura += *pitemValor;
                            break;
                    }
                }
            }
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
        printf("Etapas realizadas: %d\n",etapaAtual);
        mostrar_mapa(mapa, tamanho);

    return 0;
}
