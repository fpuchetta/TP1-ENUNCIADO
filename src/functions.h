#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "tp1.h"

#define ERROR_PARSEO -2
#define ERROR_MEMORIA -1

#define COLUMNAS_CSV 6
#define MAX_TIPO 4
#define FACTOR_AUMENTO_TAMANIO 2
#define CAPACIDAD_MINIMA 10

typedef enum operacion {
    UNION,
    INTERSECCION,
    DIFERENCIA
}operacion_t;


char* mi_strdup(const char* s);

size_t int_tam(int n);

size_t calcular_largo_linea(struct pokemon* p);

int parsear_tipo(const char tipo[MAX_TIPO]);

const char* calcular_tipo(struct pokemon* p);

struct pokemon* pokemon_clonar(struct pokemon* p);

#endif // FUNCTIONS_H_