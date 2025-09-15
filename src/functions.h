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

#define CANTIDAD_COMANDOS 5

typedef struct {
	struct pokemon **v;
	size_t n;
	size_t cap;
	int error;
} recolector_t;

typedef struct comando {
	const char *nombre;
	int (*f)(tp1_t *, int, char **);
} comando_t;

typedef enum operacion { UNION, INTERSECCION, DIFERENCIA } operacion_t;

int cmp_pokemon_nombre_ptr(void *pa, void *pb);

int cmp_pokemon_id_ptr(void *pa, void *pb);

void ordenar_vec_pokemones(void *vec, size_t tamanio, size_t tamanio_elemento,
			   int (*cmp)(void *, void *));

char *mi_strdup(const char *s);

size_t int_tam(int n);

size_t calcular_largo_linea(struct pokemon *p);

int parsear_tipo(const char tipo[MAX_TIPO]);

const char *calcular_tipo(struct pokemon *p);

struct pokemon *pokemon_clonar(struct pokemon *p);

int accionar_comando(tp1_t *tp1, const comando_t *comandos, int argc,
		     char **argv);

int operacion_con_tps(tp1_t *un_tp, tp1_t *otro_tp, const char *csv_resultado,
		      tp1_t *(*operacion)(tp1_t *, tp1_t *));

int union_pokemones(tp1_t *tp, int argc, char **argv);

int interseccion_pokemones(tp1_t *tp, int argc, char **argv);

int diferencia_pokemones(tp1_t *tp, int argc, char **argv);
#endif // FUNCTIONS_H_