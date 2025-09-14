#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "tp1.h"
#include <string.h>

#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"

#define ERROR_PARSEO -2
#define ERROR_MEMORIA -1

#define COLUMNAS_CSV 6
#define MAX_TIPO 4
#define FACTOR_AUMENTO_TAMANIO 2
#define CAPACIDAD_MINIMA 10

typedef enum operacion { UNION, INTERSECCION, DIFERENCIA } operacion_t;

int cmp_por_nombre(const struct pokemon *pa, const struct pokemon *pb);

int cmp_por_id(const struct pokemon *pa, const struct pokemon *pb);

//PROBLEMA ENCONTRADO: no puedo ordenar genericamente porq no tengo acceso a la estructura de tp1
void ordenar_vec_pokemones(struct pokemon** pokemones,size_t cantidad,int (*cmp)(const struct pokemon *,const struct pokemon *));

char *mi_strdup(const char *s);

size_t int_tam(int n);

size_t calcular_largo_linea(struct pokemon *p);

int parsear_tipo(const char tipo[MAX_TIPO]);

const char *calcular_tipo(struct pokemon *p);

struct pokemon *pokemon_clonar(struct pokemon *p);

#endif // FUNCTIONS_H_