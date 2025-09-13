#include <stdio.h>
#include "src/tp1.h"
#include "archivo.h"

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

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Uso: %s <archivo.csv> <operacion> [modo] y [valor]\n",
		       argv[0]);
		return -1;
	} else if (argc > 5) {
		printf("Utilize como maximo 5 argumentos!\n");
		return 1;
	}

	tp1_t *pokedex1 = tp1_leer_archivo(argv[1]);
	if (!pokedex1) {
		printf("Error al abrir el archivo %s\n", argv[1]);
		return 1;
	}
	
	tp1_t *pokedex2 = tp1_leer_archivo(argv[2]);
	if (!pokedex2) {
		printf("Error al abrir el archivo %s\n", argv[2]);
		return 1;
	}
	
	
	tp1_t* uniontps=tp1_union(pokedex1,pokedex1);
	if (!uniontps){
		printf("error union con el mismo tp\n");
	}
	//tp1_guardar_archivo(uniontps,"interseccion.csv");

	tp1_destruir(pokedex1);
	tp1_destruir(pokedex2);
	tp1_destruir(uniontps);
	printf("Finalizacion correcta\n");

	return 0;
}
