#include "functions.h"

#include <stdio.h>
#include <string.h>

char *mi_strdup(const char *s)
{
	if (!s)
		return NULL;
	size_t len = strlen(s) + 1;
	char *copia = malloc(len);
	if (!copia)
		return NULL;
	memcpy(copia, s, len);
	return copia;
}

size_t int_tam(int n)
{
	size_t len = 0;
	if (n == 0)
		return 1;

	while (n > 0) {
		len++;
		n /= 10;
	}
	return len;
}

size_t calcular_largo_linea(struct pokemon *p)
{
	size_t tam = int_tam(p->id);
	tam += int_tam(p->ataque);
	tam += int_tam(p->defensa);
	tam += int_tam(p->velocidad);
	tam += strlen(p->nombre);

	tam += 5;
	tam += 1;
	tam += 1;
	return tam;
}

int parsear_tipo(const char tipo[MAX_TIPO])
{
	if (strcmp(tipo, "ELEC") == 0)
		return TIPO_ELEC;
	if (strcmp(tipo, "FUEG") == 0)
		return TIPO_FUEG;
	if (strcmp(tipo, "PLAN") == 0)
		return TIPO_PLAN;
	if (strcmp(tipo, "AGUA") == 0)
		return TIPO_AGUA;
	if (strcmp(tipo, "NORM") == 0)
		return TIPO_NORM;
	if (strcmp(tipo, "FANT") == 0)
		return TIPO_FANT;
	if (strcmp(tipo, "PSI") == 0)
		return TIPO_PSI;
	if (strcmp(tipo, "LUCH") == 0)
		return TIPO_LUCH;

	return ERROR_PARSEO; // no coincide con ninguno
}

const char *calcular_tipo(struct pokemon *p)
{
	switch (p->tipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return "DESCONOCIDO";
	}
}

struct pokemon *pokemon_clonar(struct pokemon *p)
{
	if (!p)
		return NULL;

	struct pokemon *nuevo = malloc(sizeof *nuevo);
	if (!nuevo)
		return NULL;

	*nuevo = *p;

	if (p->nombre) {
		nuevo->nombre = mi_strdup(p->nombre);
		if (!nuevo->nombre) {
			free(nuevo);
			return NULL;
		}
	}

	return nuevo;
}