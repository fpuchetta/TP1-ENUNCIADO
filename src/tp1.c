#include "tp1.h"
#include <stdio.h>

#include "archivo.h"

#define COLUMNAS_CSV 6
#define MAX_TIPO 4

typedef enum {
    ERROR_PARSEO= -2,
	ERROR_MEMORIA = -1,
} estado_t;

struct tp1{
    struct pokemon **pokemones;
	int cantidad_pokemones;
};

/*
enum tipo_pokemon {
	TIPO_ELEC,
	TIPO_FUEG,
	TIPO_PLAN,
	TIPO_AGUA,
	TIPO_NORM,
	TIPO_FANT,
	TIPO_PSI,
	TIPO_LUCH
};

struct pokemon {
	int id;
	char *nombre;
	enum tipo_pokemon tipo;
	int ataque;
	int defensa;
	int velocidad;
};
*/

size_t buscar_largo_nombre(const char* linea){
    const char *inicio = strchr(linea, ';');
    if (!inicio) return 0;

    const char *fin = strchr(inicio + 1, ';');
    if (!fin) return 0;

    return fin - (inicio + 1);
}

char* reservar_memoria_nombre(const char* linea){
    size_t largo_nombre = buscar_largo_nombre(linea);
    if (largo_nombre==0) return NULL;

    char* nombre = malloc(largo_nombre+1);
    return nombre;
}

size_t contar_largo_tipo(const char* linea) {
    const char *primero = strchr(linea, ';');
    if (!primero) return 0;

    const char *segundo = strchr(primero + 1, ';');
    if (!segundo) return 0;

    const char *tercero = strchr(segundo + 1, ';');
    if (!tercero) return 0;

    return (size_t)(tercero - (segundo + 1));
}

/**
 * Dado un string en formato CSV (mirar enunciado), crea un pokemon con los
 * datos del mismo. Devuelve el pokemon creado o NULL en caso de error.
 */
struct pokemon *parsear_pokemon(const char *linea,int* estado_error){
    if (!linea) return NULL;

    struct pokemon* pokemon=calloc(1,sizeof(struct pokemon));
    if (!pokemon){
        *estado_error=ERROR_MEMORIA;
        return NULL;
    }

    pokemon->nombre=reservar_memoria_nombre(linea);
    if (!pokemon->nombre){
        free(pokemon);
        *estado_error=ERROR_MEMORIA;
        return NULL;
    }

    size_t largo_tipo=contar_largo_tipo(linea);
    if (largo_tipo > MAX_TIPO){
        free(pokemon->nombre);
        free(pokemon);
        *estado_error=ERROR_PARSEO;
        return NULL;
    }

    char tipo_aux[MAX_TIPO];

    int columnas_leidas= sscanf(linea, "%u;%[^;];%[^;];%u;%u;%u", &pokemon->id,pokemon->nombre, tipo_aux,&pokemon->ataque, &pokemon->defensa,&pokemon->velocidad);
    if (columnas_leidas != COLUMNAS_CSV){
        free(pokemon->nombre);
        free(pokemon);
        return NULL;
    }

    if ((pokemon->tipo=cargar_tipo(pokemon,tipo_aux))==ERROR_PARSEO){
        free(pokemon->nombre);
        free(pokemon);
        *estado_error=ERROR_PARSEO;
        return NULL;
    }

    return pokemon;
}

bool cargar_pokemon()

/*
    Pre: Los parametros no deben ser NULL.
    Post: Devuelve true en caso de que se hayan podido cargar los pokemones en la pokedex, devuelve false en caso contrario.
*/
bool cargar_pokemones(archivo_t* archivo, tp1_t* tp1){
    const char* linea_proxima;
    int estado_actual=0;
    while ((linea_proxima = archivo_leer_linea(archivo)) != NULL) {
        struct pokemon* p=parsear_pokemon(linea_proxima,estado_actual);
        if (!p && estado_actual==ERROR_MEMORIA)
            return false;
        else if (p){
            if (!cargar_pokemon(p)) return false;// no se permiten duplicados
            tp1->cantidad_pokemones++;
        }
    }
    return true;
}

/**
 * Lee el archivo indicado y devuelve la estructura tp1 con los pokemones.
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_leer_archivo(const char *nombre){
    if (!nombre) return NULL;

    tp1_t* pokedex=malloc(sizeof(tp1_t));
    if (!pokedex) return NULL;

    archivo_t* archivo= abrir_archivo(nombre);
    if (!archivo){
        free(pokedex);
        return NULL;
    }

    if (!cargar_pokemones(archivo,pokedex)){
        free(pokedex);
        archivo_cerrar(archivo);
        return NULL;
    }

    return pokedex;
}

/**
 * Devuevle la cantidad de pokemones leidos correctamente.
 * En caso de error devuelve 0.
*/
size_t tp1_cantidad(tp1_t *tp1);

/**
 * Guarda en el archivo indicado los pokemones contenidos en la estructura tp1 
 * de manera tal que tp1_leer_archivo pueda volver a leerlo correctamente.
 *
 * Devuelve NULL en caso de error o el tp1 pasado por parámetro en caso de exito.
*/
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre);

/**
 * Dados 2 tp1, devuelve un nuevo tp1 que es la unión de ambos.
 * No se deben incluír pokemones repetidos. En caso de repeticiones se debe
 * preferir el pokemon del primer tp1.
 *
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp);

/**
 * Dados 2 tp1, devuelve un nuevo tp1 que es la intersección de ambos.
 * 
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp);

/**
* Dados 2 tp1, devuelve un nuevo tp1 que es la diferencia de ambos.
* La diferencia se define como los pokemones que están en el primer tp1
* pero no en el segundo.
*
* En caso de error devuelve NULL.
*/
tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp);

/**
* Busca un pokemon por nombre.
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre);

/**
* Busca un pokemon por id.
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_id(tp1_t *tp, int id);

/**
 * Aplica la función f a cada pokemon por orden de id (de menor a mayor).
 * La función deja de aplicarse si f devuelve false o se terminaron los pokemones.
 * 
 * Devuelve la cantidad de pokemones sobre los cuales se aplicó la función f.
 */
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra);

/**
 * Libera toda la memoria asociada al tp1
 */
void tp1_destruir(tp1_t *tp1);