#include "tp1.h"
#include <stdio.h>
#include <string.h>

#include "archivo.h"

#define COLUMNAS_CSV 6
#define MAX_TIPO 4
#define FACTOR_AUMENTO_TAMANIO 2
#define CAPACIDAD_MINIMA 10

#define ERROR_PARSEO -2
#define ERROR_MEMORIA -1


struct tp1{
    struct pokemon **pokemones;
	size_t cantidad_pokemones;
    size_t capacidad;
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

size_t buscar_largo_nombre(const char* linea) {
    const char *inicio = strchr(linea, ',');
    if (!inicio) return 0;

    inicio++;

    const char *fin = strchr(inicio, ',');
    if (!fin) return 0;

    return (size_t)(fin - inicio);
}

char* reservar_memoria_nombre(const char* linea){
    size_t largo_nombre = buscar_largo_nombre(linea);
    if (largo_nombre==0) return NULL;

    char* nombre = malloc(largo_nombre+1);
    return nombre;
}

size_t contar_largo_tipo(const char* linea) {
    const char *primero = strchr(linea, ',');
    if (!primero) return 0;

    const char *segundo = strchr(primero + 1, ',');
    if (!segundo) return 0;

    const char *tercero = strchr(segundo + 1, ',');
    if (!tercero) return 0;

    return (size_t)(tercero - (segundo + 1));
}

int parsear_tipo(const char tipo[MAX_TIPO]) {
    if (strcmp(tipo, "ELEC") == 0) return TIPO_ELEC;
    if (strcmp(tipo, "FUEG") == 0) return TIPO_FUEG;
    if (strcmp(tipo, "PLAN") == 0) return TIPO_PLAN;
    if (strcmp(tipo, "AGUA") == 0) return TIPO_AGUA;
    if (strcmp(tipo, "NORM") == 0) return TIPO_NORM;
    if (strcmp(tipo, "FANT") == 0) return TIPO_FANT;
    if (strcmp(tipo, "PSI") == 0) return TIPO_PSI;
    if (strcmp(tipo, "LUCH") == 0) return TIPO_LUCH;

    return ERROR_PARSEO;  // no coincide con ninguno
}

void destruir_pokemon(struct pokemon *p) {
    if (!p) return;
    free(p->nombre);
    free(p);
}


struct pokemon* reservar_memoria_parseo(const char* linea,int* estado_error){
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

    return pokemon;
}

int completar_pokemon(const char* linea, struct pokemon* p, char tipo_aux[MAX_TIPO], int* estado_error){
    size_t largo_tipo=contar_largo_tipo(linea);
    if (largo_tipo > MAX_TIPO){
        destruir_pokemon(p);
        *estado_error=ERROR_PARSEO;
    }

    int columnas_leidas= sscanf(linea, "%d,%[^,],%[^,],%d,%d,%d", &p->id,p->nombre, tipo_aux, &p->ataque, &p->defensa, &p->velocidad);

    return columnas_leidas;
}

void verificar_errores(struct pokemon* p,char tipo_aux[MAX_TIPO],int col_leidas,int* estado_error){
    p->tipo=parsear_tipo(tipo_aux);
    
    if (p->id < 0 || p->ataque < 0 ||
        p->defensa < 0 || p->velocidad < 0 || col_leidas != COLUMNAS_CSV || p->tipo==ERROR_PARSEO) {
        destruir_pokemon(p);
        *estado_error=ERROR_PARSEO;
    }
}

/**
 * Dado un string en formato CSV (mirar enunciado), crea un pokemon con los
 * datos del mismo. Devuelve el pokemon creado o NULL en caso de error.
 */
struct pokemon *parsear_pokemon(const char *linea,int* estado_error){
    if (!linea || strcmp(linea,"")==0){
        *estado_error=ERROR_PARSEO;
        return NULL;
    }

    struct pokemon* pokemon=reservar_memoria_parseo(linea,estado_error);
    if (!pokemon) return NULL;

    char tipo_aux[MAX_TIPO];
    int columnas_leidas = completar_pokemon(linea, pokemon, tipo_aux, estado_error);
    if (*estado_error == ERROR_PARSEO) return NULL;

    verificar_errores(pokemon,tipo_aux,columnas_leidas,estado_error);
    if(*estado_error==ERROR_PARSEO) return NULL;

    return pokemon;
}

bool cargar_pokemon(struct pokemon* p, tp1_t* tp1){
    if (tp1->cantidad_pokemones == tp1->capacidad) {
        size_t nueva_cap = tp1->capacidad ? tp1->capacidad * FACTOR_AUMENTO_TAMANIO : CAPACIDAD_MINIMA;  // arranca con 4
        
        struct pokemon **temp = realloc(tp1->pokemones, nueva_cap * sizeof(struct pokemon*));
        if (!temp) return false;

        tp1->pokemones = temp;
        tp1->capacidad = nueva_cap;
    }

    tp1->pokemones[tp1->cantidad_pokemones] = p;  // lo guardo en la posición libre
    tp1->cantidad_pokemones++;                    // incremento cantidad cargada
    return true;
}

bool esta_cargado(tp1_t* tp1,int id_buscada){
    int i=0;
    bool encontrado=false;
    while (i<tp1->cantidad_pokemones && !encontrado){
        if (tp1->pokemones[i]->id == id_buscada){
            encontrado=true;
        }
        i++;
    }
    return encontrado;
}

bool incorporacion_correcta(struct pokemon* p, tp1_t* tp1, int* estado_actual){
    bool agregado = false;
    if (p) {
        if (!esta_cargado(tp1, p->id)) {
            if (cargar_pokemon(p, tp1)) {
                agregado = true;
                *estado_actual = 0;
            }else
                *estado_actual = ERROR_MEMORIA;
        }
        if (!agregado)
            destruir_pokemon(p);
    }
    if (*estado_actual == ERROR_MEMORIA)
        return false;
    
    *estado_actual = 0;
    return true;
}

/*
    Pre: Los parametros no deben ser NULL.
    Post: Devuelve true en caso de que se hayan podido cargar los pokemones en la pokedex, devuelve false en caso contrario.
*/
bool cargar_pokemones(archivo_t* archivo, tp1_t* tp1){
    const char* linea_proxima;
    int estado_actual=0;
	while (archivo_hay_mas_lineas(archivo)) {
		linea_proxima = archivo_leer_linea(archivo);
        struct pokemon* p=parsear_pokemon(linea_proxima,&estado_actual);

        if (!incorporacion_correcta(p,tp1,&estado_actual)) return false;
    }

    return true;
}

void ordenar_pokemones(tp1_t* tp1){
    size_t n=tp1->cantidad_pokemones;
    struct pokemon** pokemons=tp1->pokemones;

    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (pokemons[j]->id > pokemons[j + 1]->id) {
                struct pokemon *tmp = pokemons[j];
                pokemons[j] = pokemons[j + 1];
                pokemons[j + 1] = tmp;
            }
        }
    }
}

/**
 * Lee el archivo indicado y devuelve la estructura tp1 con los pokemones.
 * En caso de error devuelve NULL.
*/
tp1_t *tp1_leer_archivo(const char *nombre){
    if (!nombre) return NULL;

    tp1_t* pokedex=calloc(1,sizeof(tp1_t));
    if (!pokedex) return NULL;

    archivo_t* archivo= archivo_abrir(nombre);
    if (!archivo){
        free(pokedex);
        return NULL;
    }

    if (!cargar_pokemones(archivo,pokedex)){
        tp1_destruir(pokedex);
        archivo_cerrar(archivo);
        return NULL;
    }

    if (pokedex->cantidad_pokemones>1)
        ordenar_pokemones(pokedex);

    archivo_cerrar(archivo);
    return pokedex;
}

/**
 * Devuevle la cantidad de pokemones leidos correctamente.
 * En caso de error devuelve 0.
*/
size_t tp1_cantidad(tp1_t *tp1){
    if (!tp1) return 0;
    return tp1->cantidad_pokemones;
}

size_t int_len(int n) {
    size_t len = 0;
    if (n == 0) return 1;   // "0" ocupa 1 caracter
    if (n < 0) { 
        len++;              // espacio para el signo '-'
        n = -n;
    }
    while (n > 0) {
        len++;
        n /= 10;
    }
    return len;
}

const char* parsear_linea(struct pokemon* p){
    int i=atoi(p.),t=0,a=0,d=0,v=0;
    
    const char* linea=reservar_memoria_linea();
}

tp1_t* llenar_archivo(archivo_t* archivo,tp1_t* tp1){
    const char* linea;
    for (int i=0;i<tp1->cantidad_pokemones;i++){
        linea=parsear_linea(tp1->pokemones[i]);
    }
}

/**
 * Guarda en el archivo indicado los pokemones contenidos en la estructura tp1 
 * de manera tal que tp1_leer_archivo pueda volver a leerlo correctamente.
 *
 * Devuelve NULL en caso de error o el tp1 pasado por parámetro en caso de exito.
*/
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre){
    if (!tp1 || !nombre) return NULL;

    archivo_t* archivo=archivo_abrir(nombre);
    if (!archivo) return NULL;

    return llenar_archivo(archivo,tp1);
}

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
struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre){
    if (!tp || !nombre) return NULL;

    int i=0;
    struct pokemon* pokemon=NULL;
    while (i < tp->cantidad_pokemones && !pokemon){
        if (strcmp(nombre,tp->pokemones[i]->nombre)==0){
            pokemon=tp->pokemones[i];
        }
        i++;
    }
    return pokemon;
}

/**
* Busca un pokemon por id.
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_id(tp1_t *tp, int id){
    if (!tp) return NULL;
    size_t inicio = 0;
    size_t fin = tp->cantidad_pokemones;

    while (inicio < fin) {
        size_t medio = inicio + (fin - inicio) / 2;

        if (tp->pokemones[medio]->id == id) {
            return tp->pokemones[medio];
        } else if (tp->pokemones[medio]->id < id) {
            inicio = medio + 1;
        } else {
            fin = medio;
        }
    }
    return NULL;
}

/**
 * Aplica la función f a cada pokemon por orden de id (de menor a mayor).
 * La función deja de aplicarse si f devuelve false o se terminaron los pokemones.
 * 
 * Devuelve la cantidad de pokemones sobre los cuales se aplicó la función f.
 */
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra){
    if (!un_tp || !f) return 0;

	size_t indice = 0;
	bool iteracion_terminada = false;
	while (indice < un_tp->cantidad_pokemones && !iteracion_terminada) {
		if (!f(un_tp->pokemones[indice], extra)) {
			iteracion_terminada = true;
		}
		indice++;
	}
    return indice;
}

/**
 * Libera toda la memoria asociada al tp1
 */
void tp1_destruir(tp1_t *tp1){
    if (!tp1) return;

    for (int i=0;i<tp1->cantidad_pokemones;i++){
        destruir_pokemon(tp1->pokemones[i]);
    }
    free(tp1->pokemones);
    free(tp1);
}