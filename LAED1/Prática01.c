#include <stdio.h>
#include <math.h>

void calcula_hexagono(float l, float *area, float *perimetro) {
    *area = (3 * pow(l, 2) * sqrt(3)) / 2;
    *perimetro = 6 * l;
    printf ("Área: %f\nPerímetro: %f", *area, *perimetro);
}

int main (void) {
    float l, area, perimetro;
    float *aarea = &area, *pperimetro = &perimetro;

    printf ("Informe o valor do lado do hexágono.");
    scanf ("%f", &l);
    calcula_hexagono (l, aarea, pperimetro);

return 0;
}