#include <stdio.h>
void testetesouro (int *ptesouro) {
    if (*ptesouro==0) {
        printf ("TESOURO: INATIVO\n");
    } else {
        printf ("TESOURO: ATIVO\n");
    }
}
void vidaatual (int *pvida) {
    printf ("VIDA: %d\n", *pvida);
}
int main () {
    int vida=100;
    int tesouro=0;
    int *pvida = &vida;
    int *ptesouro = &tesouro;

    vidaatual (pvida);
    testetesouro (ptesouro);
    *pvida=*pvida-40;
    
    return 0;
}