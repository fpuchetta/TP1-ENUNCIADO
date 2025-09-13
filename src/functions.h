#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "tp1.h"

#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"

#define ERROR_PARSEO -2
#define ERROR_MEMORIA -1

#define COLUMNAS_CSV 6
#define MAX_TIPO 4
#define FACTOR_AUMENTO_TAMANIO 2
#define CAPACIDAD_MINIMA 10

typedef enum operacion { UNION, INTERSECCION, DIFERENCIA } operacion_t;

/*

PROBLEMA ENCONTRADO: no puedo ordenar genericamente porq no tengo acceso a la estructura de tp1
void ordenar_poks_generico(tp1_t* tp1,int (*cmp)(struct pokemon *, struct pokemon *)){
    if (!tp1 || !cmp) return;

    size_t n = tp1_cantidad(tp1);
    if (n < 2) return;

    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (comparador()) {
                struct pokemon *tmp = pokemons[j];
                pokemons[j] = pokemons[j + 1];
                pokemons[j + 1] = tmp;
            }
        }
    }
}
*/

char *mi_strdup(const char *s);

size_t int_tam(int n);

size_t calcular_largo_linea(struct pokemon *p);

int parsear_tipo(const char tipo[MAX_TIPO]);

const char *calcular_tipo(struct pokemon *p);

struct pokemon *pokemon_clonar(struct pokemon *p);

#endif // FUNCTIONS_H_