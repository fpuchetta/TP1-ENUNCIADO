#include <stdio.h>
#include "src/functions.h"
#include <string.h>

#define CANTIDAD_COMANDOS 5


typedef struct {
    struct pokemon **v;
    size_t n;
    size_t cap;
    int error;
} recolector_t;

typedef struct comando{
	const char* nombre;
	int (*f)(tp1_t*,int, char**);
}comando_t;

void imprimir_pokemon(const struct pokemon *pokemon)
{
	printf("%s(%i) - Tipo:%i F:%u D:%u I:%u\n", pokemon->nombre,
	       pokemon->id, pokemon->tipo, pokemon->ataque, pokemon->defensa,
	       pokemon->velocidad);
}

bool pokedex_mostrar_nombres(struct pokemon *pokemon_a_evaluar, void *ctx)
{
	ctx = NULL;
	imprimir_pokemon(pokemon_a_evaluar);
	return true;
}

bool validar_argumentos(int argc, char* argv[]){
	const char *op=argv[2];
    int esperado = 0;

    if (strcmp(op, "buscar") == 0 || strcmp(op, "union") == 0 || strcmp(op, "interseccion") == 0 || strcmp(op, "diferencia") == 0)
        esperado = 5;
    else if (strcmp(op, "mostrar") == 0)
        esperado = 4;
    else {
        printf("Error: Operacion '%s' no reconocida.\n", op);
        return false;
    }

    if (argc != esperado) {
        printf("Error: cantidad de argumentos invalida para '%s'.\n", op);
        return false;
    }
	
	return true;
}

int buscar_pokemon(tp1_t* tp, int argc, char** argv){return 0;}

bool recolectar_pokemones(struct pokemon *p, void *extra) {
    recolector_t *r = extra;
    if (r->n == r->cap){
		r->error = 1;
		return false;
	}
    r->v[r->n++] = p;
    return true;
}

int mostrar_por_nombre(tp1_t *tp) {
    size_t n = tp1_cantidad(tp);
    struct pokemon **tmp = malloc(n * sizeof *tmp);
    if (!tmp) return 1;

    recolector_t r = { .v = tmp, .n = 0, .cap = n, .error = 0 };

    size_t aplicados = tp1_con_cada_pokemon(tp, recolectar_pokemones, &r);
    if (r.error || aplicados != n) {
		free(tmp);
		printf("error aca\n");
		return -1;
	}

	ordenar_vec_pokemones(tmp,n,cmp_por_nombre);

	for (size_t i = 0; i < n; i++) {
        struct pokemon *p = tmp[i];
        imprimir_pokemon(p);
    }

	free(tmp);

	return 1;
}
int mostrar_por_id(tp1_t* tp){
	size_t iterados= tp1_con_cada_pokemon(tp,pokedex_mostrar_nombres,NULL);
	if (iterados != tp1_cantidad(tp)) return -1;
	return 1;
}

int mostrar_pokemon(tp1_t* tp, int argc, char** argv){
	if (strcmp(argv[3],"id")==0)
		return mostrar_por_id(tp);
	else if(strcmp(argv[3],"nombre")==0)
		return mostrar_por_nombre(tp);

	return -1;
}

int union_pokemones(tp1_t* tp, int argc, char** argv){return 0;}

int interseccion_pokemones(tp1_t* tp, int argc, char** argv){return 0;}

int diferencia_pokemones(tp1_t* tp, int argc, char** argv){return 0;}

int accionar_comando(tp1_t* tp1, const comando_t* comandos, int argc, char** argv){
	for (int i=0;i<CANTIDAD_COMANDOS;i++){
		if (strcmp(argv[2],comandos[i].nombre)==0){
			return comandos[i].f(tp1,argc,argv);
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Uso:\n");
        printf("  %s <archivo.csv> buscar <id|nombre> <valor>\n", argv[0]);
        printf("  %s <archivo.csv> operacion_conjuntos <archivo2.csv> <resultado.csv>\n", argv[0]);
        printf("  %s <archivo.csv> mostrar <id|nombre>\n", argv[0]);
        return -1;
    }

	if (!validar_argumentos(argc,argv)){
		return -1;
	}

	const comando_t comandos[]={{.nombre="buscar", .f=buscar_pokemon},
						  {.nombre="mostrar", .f=mostrar_pokemon},
						  {.nombre="union", .f=union_pokemones},
						  {.nombre="interseccion", .f=interseccion_pokemones},
						  {.nombre="diferencia", .f=diferencia_pokemones}};

	tp1_t* tp1=tp1_leer_archivo(argv[1]);
	if (!tp1){
		printf("Error al abrir el archivo %s.\n",argv[1]);
		return -1;
	}

	if (accionar_comando(tp1,comandos,argc,argv) != 1){
		printf("Error al accionar el comando %s.\n",argv[2]);
		tp1_destruir(tp1);
		return -1;
	}

	tp1_destruir(tp1);
	return 0;
}
