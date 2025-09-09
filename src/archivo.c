#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archivo.h"

const int TAM_LINEA_INICIAL = 0;
const int CANT_LINEAS_INICIAL = 0;

struct archivo {
	FILE *apertura;
	char *linea_leida;
	int tam_linea;
	int cant_lineas;
};

archivo_t *archivo_abrir(const char *nombre)
{
	archivo_t *archivo_abierto = malloc(sizeof(archivo_t));
	if (archivo_abierto == NULL)
		return NULL;

	archivo_abierto->apertura = fopen(nombre, "r");
	if (archivo_abierto->apertura == NULL) {
		free(archivo_abierto);
		return NULL;
	}
	archivo_abierto->linea_leida = NULL;
	archivo_abierto->tam_linea = TAM_LINEA_INICIAL;
	archivo_abierto->cant_lineas = CANT_LINEAS_INICIAL;
	return archivo_abierto;
}

char *guardar_memoria_linea(archivo_t *archivo, int tamanio)
{
    char *linea_temporal = realloc(archivo->linea_leida, (size_t)tamanio);
    if (linea_temporal == NULL)
        return NULL;
    return linea_temporal;
}

void quitar_salto_de_linea(archivo_t *archivo, int tamanio_linea)
{
	if (tamanio_linea > 0 &&
	    archivo->linea_leida[tamanio_linea - 1] == '\n') {
		archivo->linea_leida[tamanio_linea - 1] = '\0';
		tamanio_linea--;
	}
}

const char *archivo_leer_linea(archivo_t *archivo)
{
	archivo->tam_linea = 10;
	archivo->linea_leida =
		guardar_memoria_linea(archivo, archivo->tam_linea);
	if (archivo->linea_leida == NULL)
		return NULL;
	char* linea_temporal=fgets(archivo->linea_leida,archivo->tam_linea,archivo->apertura);
	if (linea_temporal==NULL)
		return NULL;
	archivo->linea_leida=linea_temporal;
	archivo->tam_linea*=2;
	archivo->linea_leida=guardar_memoria_linea(archivo,archivo->tam_linea);
	archivo->linea_leida=fgets(archivo->linea_leida,archivo->tam_linea,archivo->apertura);
	return archivo->linea_leida;
}

int archivo_hay_mas_lineas(archivo_t *archivo)
{
    return (archivo == NULL || feof(archivo->apertura)) ? 0 : 1;
}
int archivo_lineas_leidas(archivo_t *archivo)
{
	if (archivo == NULL) {
		return 0;
	}
	return archivo->cant_lineas;
}

void archivo_cerrar(archivo_t *archivo)
{
	free(archivo->linea_leida);
	fclose(archivo->apertura);
	free(archivo);
}