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

	tp1_t *pokedex = tp1_leer_archivo(argv[1]);
	if (!pokedex) {
		printf("Error al abrir el archivo %s\n", argv[1]);
		return 1;
	}
	
	tp1_con_cada_pokemon(pokedex,pokedex_mostrar_nombres,NULL);

	tp1_destruir(pokedex);
	printf("Finalizacion correcta\n");

	return 0;
}
