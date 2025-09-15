#include "pa2m.h"
#include "src/tp1.h"
#include <string.h>

#define ARCHIVO_PRUEBA_INEXISTENTE "ejemplos/asdkasjhfskladjhfksdfhksdf.csv"

#define CSV_NOMBRE_VALIDO "ejemplos/pok_ok.csv"
#define CSV_VACIO "ejemplos/vacio.csv"
#define CSV_LARGO "ejemplos/largo.csv"
#define CSV_NORMAL "ejemplos/normal.csv"
#define CSV_MUCHOS "ejemplos/pok_muchos.csv"
#define CSV_NO_EXISTE "ejemplos/no_existe.csv"
#define CSV_NULO NULL

#define CSV_MEZCLA "ejemplos/pok_mixto.csv"
#define CSV_TODAS_INVALIDAS "ejemplos/pok_todas_invalidas.csv"
#define CSV_SIN_NEWLINE_FINAL "ejemplos/pok_sin_nl_final.csv"

#define OUT_GUARDAR_OK "ejemplos/tp1_guardar_ok.csv"
#define OUT_GUARDAR_OVERWRITE "ejemplos/tp1_guardar_over.csv"
#define OUT_GUARDAR_VACIO "ejemplos/tp1_guardar_vacio.csv"
#define OUT_NOEXISTE "ejemplos/no_such_dir/tp.csv"

#define ESP_OK_CANTIDAD 3
#define ESP_VACIO_CANTIDAD 0
#define ESP_LARGO_CANTIDAD 15
#define ESP_NORMAL_CANTIDAD 5
#define ESP_MUCHOS_CANTIDAD 20

#define ESP_MEZCLA_VALIDOS 2
#define ESP_TODAS_INVALIDAS_CANT 0
#define ESP_SIN_NL_FINAL_CANT 4

struct recolector_ids {
	int *ids;
	size_t cantidad_ids;
	size_t capacidad;
	size_t cortar_en;
};
bool recolectar_ids(struct pokemon *p, void *extra)
{
	struct recolector_ids *r = (struct recolector_ids *)extra;
	if (!p || !r)
		return false;

	if (r->cantidad_ids < r->capacidad)
		r->ids[r->cantidad_ids++] = p->id;

	if (r->cortar_en && r->cantidad_ids >= r->cortar_en)
		return false;

	return true;
}

bool contar_pokemons(struct pokemon *p, void *extra)
{
	(void)p;
	if (!extra)
		return true;

	int *cantidad = (int *)extra;
	(*cantidad)++;
	return true;
}

bool estan_ordenados_por_id(const int *v, size_t n)
{
	if (!v)
		return false;
	for (size_t i = 1; i < n; ++i)
		if (v[i - 1] > v[i])
			return false;
	return true;
}

struct ctx_inter {
	tp1_t *b;
	size_t c;
};
bool contar_si_esta_en_b(struct pokemon *p, void *extra)
{
	struct ctx_inter *ctx = (struct ctx_inter *)extra;
	if (tp1_buscar_id(ctx->b, p->id))
		ctx->c++;
	return true;
}

struct ctx_diff {
	tp1_t *b;
	size_t c;
};
bool contar_si_no_esta_en_b(struct pokemon *p, void *extra)
{
	struct ctx_diff *ctx = (struct ctx_diff *)extra;
	if (!tp1_buscar_id(ctx->b, p->id))
		ctx->c++;
	return true;
}

void tp1_leer_archivo_nombre_valido()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Csv con nombre valido no devuelve null");
	if (tp) {
		pa2m_afirmar(
			tp1_cantidad(tp) == ESP_OK_CANTIDAD,
			"Cantidad de pokemons insertados correctamente (%d)",
			ESP_OK_CANTIDAD);
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_archivo_vacio_crea_pokedex_vacia()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL, "Csv vacio se crea correctamente");
	if (tp) {
		pa2m_afirmar(
			tp1_cantidad(tp) == ESP_VACIO_CANTIDAD,
			"Csv vacio crea un tp1 con cantidad 0 de pokemones");
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_archivo_largo_crea_correctamente()
{
	tp1_t *tp = tp1_leer_archivo(CSV_LARGO);
	pa2m_afirmar(tp != NULL,
		     "Csv grande no dificulta creacion de estructura");
	if (tp) {
		pa2m_afirmar(
			tp1_cantidad(tp) == ESP_LARGO_CANTIDAD,
			"Cuenta correcta de elementos en archivo largo (%d)",
			ESP_LARGO_CANTIDAD);
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_archivo_normal_crea_con_cantidad_correcta()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(
		tp != NULL,
		"CSV con cantidad normal de pokemones se crea correctamente");
	if (tp) {
		pa2m_afirmar(tp1_cantidad(tp) == ESP_NORMAL_CANTIDAD,
			     "Cantidad correcta en archivo normal (%d)",
			     ESP_NORMAL_CANTIDAD);
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_muchos_pokemones_cuenta_bien()
{
	tp1_t *tp = tp1_leer_archivo(CSV_MUCHOS);
	pa2m_afirmar(tp != NULL,
		     "CSV de muchas lineas crea la estructura valida");
	if (tp) {
		pa2m_afirmar(tp1_cantidad(tp) == ESP_MUCHOS_CANTIDAD,
			     "Cantidad de pokemones esperados (%d)",
			     ESP_MUCHOS_CANTIDAD);
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_archivo_no_existente()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NO_EXISTE);
	pa2m_afirmar(tp == NULL, "Csv inexistente devuelve NULL");
}

void tp1_leer_archivo_archivo_nulo()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NULO);
	pa2m_afirmar(tp == NULL, "Parametro nombre NULL devuelve NULL");
}

void tp1_leer_archivo_mezcla_validos_invalidos()
{
	tp1_t *tp = tp1_leer_archivo(CSV_MEZCLA);
	pa2m_afirmar(
		tp != NULL,
		"Csv con lineas validas como invalidas se crea correctamente");
	if (tp) {
		pa2m_afirmar(tp1_cantidad(tp) == ESP_MEZCLA_VALIDOS,
			     "Cantidad de lineas validas correctamente (%d)",
			     ESP_MEZCLA_VALIDOS);
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_todas_lineas_invalidas()
{
	tp1_t *tp = tp1_leer_archivo(CSV_TODAS_INVALIDAS);
	pa2m_afirmar(tp != NULL,
		     "Csv totalmente invalido crea estructura correctamente");
	if (tp) {
		pa2m_afirmar(
			tp1_cantidad(tp) == ESP_TODAS_INVALIDAS_CANT,
			"Cantidad 0 cuando todas las lineas son invalidas (%d)",
			ESP_TODAS_INVALIDAS_CANT);
		tp1_destruir(tp);
	}
}

void tp1_leer_archivo_linea_final_sin_newline()
{
	tp1_t *tp = tp1_leer_archivo(CSV_SIN_NEWLINE_FINAL);
	pa2m_afirmar(tp != NULL,
		     "Csv con ultima linea sin '\\n' se procesa correctamente");
	if (tp) {
		pa2m_afirmar(
			tp1_cantidad(tp) == ESP_SIN_NL_FINAL_CANT,
			"Cuenta correcta incluyendo la ultima linea sin newline (%d)",
			ESP_SIN_NL_FINAL_CANT);
		tp1_destruir(tp);
	}
}

void pruebas_tp1_leer_archivo()
{
	pa2m_nuevo_grupo("TP1 leer archivo parametros validos");
	tp1_leer_archivo_nombre_valido();
	tp1_leer_archivo_archivo_vacio_crea_pokedex_vacia();
	tp1_leer_archivo_archivo_largo_crea_correctamente();
	tp1_leer_archivo_archivo_normal_crea_con_cantidad_correcta();
	tp1_leer_archivo_muchos_pokemones_cuenta_bien();

	pa2m_nuevo_grupo("TP1 leer archivo parametros invalidos");
	tp1_leer_archivo_archivo_no_existente();
	tp1_leer_archivo_archivo_nulo();
	tp1_leer_archivo_mezcla_validos_invalidos();
	tp1_leer_archivo_todas_lineas_invalidas();
	tp1_leer_archivo_linea_final_sin_newline();
}
//----

void tp1_cantidad_tp_valido_devuelve_cantidad_correcta()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Csv valido devuelve estructura no-NULL");
	if (!tp)
		return;

	size_t cant = tp1_cantidad(tp);
	pa2m_afirmar(
		cant == ESP_OK_CANTIDAD,
		"tp1_cantidad devuelve cantidad correcta para CSV valido (%d)",
		ESP_OK_CANTIDAD);

	tp1_destruir(tp);
}

void tp1_cantidad_csv_mixto_cuenta_solo_validos()
{
	tp1_t *tp = tp1_leer_archivo(CSV_MEZCLA);
	pa2m_afirmar(tp != NULL,
		     "CSV mixto (validos + invalidos) crea estructura no-NULL");
	if (!tp)
		return;

	size_t cant = tp1_cantidad(tp);
	pa2m_afirmar(cant == ESP_MEZCLA_VALIDOS,
		     "tp1_cantidad cuenta solo las lineas validas (%d)",
		     ESP_MEZCLA_VALIDOS);

	tp1_destruir(tp);
}

void tp1_cantidad_archivo_grande_coincide()
{
	tp1_t *tp = tp1_leer_archivo(CSV_LARGO);
	pa2m_afirmar(tp != NULL, "CSV grande crea estructura no-NULL");
	if (!tp)
		return;

	size_t cant = tp1_cantidad(tp);
	pa2m_afirmar(
		cant == ESP_LARGO_CANTIDAD,
		"tp1_cantidad cuenta cantidad correcta en archivo grande (%d)",
		ESP_LARGO_CANTIDAD);

	tp1_destruir(tp);
}

void tp1_cantidad_tp_vacio_devuelve_cero()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL,
		     "Archivo existente pero vacio crea estructura no-NULL");
	if (!tp)
		return;

	size_t cant = tp1_cantidad(tp);
	pa2m_afirmar(
		cant == ESP_VACIO_CANTIDAD,
		"tp1_cantidad cuenta cantidad 0 cuando el archivo esta vacio");

	tp1_destruir(tp);
}

void tp1_cantidad_tp_null_devuelve_cero()
{
	pa2m_afirmar(tp1_cantidad(NULL) == 0,
		     "tp1_cantidad con null devuelve cero");
}

void pruebas_tp1_cantidad()
{
	pa2m_nuevo_grupo("TP1 cantidad parametros validos");
	tp1_cantidad_tp_valido_devuelve_cantidad_correcta();
	tp1_cantidad_csv_mixto_cuenta_solo_validos();
	tp1_cantidad_archivo_grande_coincide();

	pa2m_nuevo_grupo("TP1 cantidad parametros invalidos");
	tp1_cantidad_tp_vacio_devuelve_cero();
	tp1_cantidad_tp_null_devuelve_cero();
}
//----

void tp1_union_mismo_tp_devuelve_otro_igual()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(a != NULL, "Tp A creado correctamente");
	if (!a)
		return;

	size_t cant_a = tp1_cantidad(a);

	tp1_t *u = tp1_union(a, a);
	pa2m_afirmar(u != NULL, "Union(A, A) devuelve estructura valida");
	if (u) {
		size_t cant_u = tp1_cantidad(u);
		pa2m_afirmar(
			cant_u == cant_a,
			"La cantidad en la union coincide con la original (%zu)",
			cant_a);

		struct pokemon *p;
		p = tp1_buscar_id(a, 1);
		if (p)
			pa2m_afirmar(tp1_buscar_id(u, 1) != NULL,
				     "id=1 esta en la union");
		p = tp1_buscar_id(a, 2);
		if (p)
			pa2m_afirmar(tp1_buscar_id(u, 2) != NULL,
				     "id=2 esta en la union");
		p = tp1_buscar_id(a, 3);
		if (p)
			pa2m_afirmar(tp1_buscar_id(u, 3) != NULL,
				     "id=3 esta en la union");
		p = tp1_buscar_id(a, 4);
		if (p)
			pa2m_afirmar(tp1_buscar_id(u, 4) != NULL,
				     "id=4 esta en la union");
		p = tp1_buscar_id(a, 5);
		if (p)
			pa2m_afirmar(tp1_buscar_id(u, 5) != NULL,
				     "id=5 esta en la union");

		tp1_destruir(u);
	}
	tp1_destruir(a);
}

void tp1_union_tp_distintos_devuelve_otro_con_suma_correcta()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	tp1_t *b = tp1_leer_archivo(CSV_MEZCLA);
	pa2m_afirmar(a && b, "Tp A y B creados correctamente");
	if (!a || !b) {
		if (a)
			tp1_destruir(a);
		if (b)
			tp1_destruir(b);
		return;
	}

	tp1_t *u = tp1_union(a, b);
	pa2m_afirmar(u != NULL, "Union(A, B) devuelve estructura valida");
	if (u) {
		pa2m_afirmar(tp1_cantidad(u) == 6,
			     "Union sin repetidos tiene %d elementos", 6);
		pa2m_afirmar(tp1_buscar_id(u, 1) != NULL, "Contiene id=1");
		pa2m_afirmar(tp1_buscar_id(u, 2) != NULL, "Contiene id=2");
		pa2m_afirmar(tp1_buscar_id(u, 3) != NULL, "Contiene id=3");
		pa2m_afirmar(tp1_buscar_id(u, 4) != NULL, "Contiene id=4");
		pa2m_afirmar(tp1_buscar_id(u, 5) != NULL, "Contiene id=5");

		tp1_destruir(u);
	}

	tp1_destruir(a);
	tp1_destruir(b);
}

void tp1_union_dos_tp_vacio_devuelve_tp_vacio()
{
	tp1_t *v1 = tp1_leer_archivo(CSV_VACIO);
	tp1_t *v2 = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(v1 && v2, "Tp A y B creados correctamente");

	tp1_t *u = tp1_union(v1, v2);
	pa2m_afirmar(u != NULL, "Union de vacios devuelve estructura valida");
	if (u) {
		pa2m_afirmar(tp1_cantidad(u) == ESP_VACIO_CANTIDAD,
			     "Cantidad 0 en la union vacia");
		tp1_destruir(u);
	}

	tp1_destruir(v1);
	tp1_destruir(v2);
}

void tp1_union_largo_suma_correctamente()
{
	tp1_t *largo = tp1_leer_archivo(CSV_LARGO);
	tp1_t *normal = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(largo && normal, "Tp A y B creados correctamente");
	if (!largo || !normal) {
		if (largo)
			tp1_destruir(largo);
		if (normal)
			tp1_destruir(normal);
		return;
	}

	tp1_t *u = tp1_union(largo, normal);
	pa2m_afirmar(u != NULL,
		     "Union(largo, normal) devuelve estructura valida");
	if (u) {
		pa2m_afirmar(tp1_cantidad(u) == 18,
			     "Cantidad correcta en la union largo+normal (%d)",
			     18);

		pa2m_afirmar(tp1_buscar_id(u, 30) != NULL,
			     "Esta id=30 (solo en largo)");
		pa2m_afirmar(tp1_buscar_id(u, 2) != NULL,
			     "Esta id=2 (solo en normal)");
		pa2m_afirmar(tp1_buscar_id(u, 17) != NULL,
			     "Esta id=17 (esta en ambos)");

		tp1_destruir(u);
	}

	tp1_destruir(largo);
	tp1_destruir(normal);
}

void tp1_union_unir_muchas_veces_sigue_contando_bien()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	tp1_t *b = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	tp1_t *v = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(a && b && v, "Tp A, B y vacio creados correctamente");
	if (!a || !b || !v) {
		if (a)
			tp1_destruir(a);
		if (b)
			tp1_destruir(b);
		if (v)
			tp1_destruir(v);
		return;
	}

	tp1_t *u1 = tp1_union(a, b);
	pa2m_afirmar(u1 != NULL, "Union(A, B) valido");
	if (!u1) {
		tp1_destruir(a);
		tp1_destruir(b);
		tp1_destruir(v);
		return;
	}

	tp1_t *u2 = tp1_union(u1, a);
	pa2m_afirmar(u2 != NULL, "(A ∪ B) ∪ A valido");

	tp1_t *u3 = u2 ? tp1_union(u2, b) : NULL;
	pa2m_afirmar(u3 != NULL, "((A ∪ B) ∪ A) ∪ B valido");

	tp1_t *u4 = u3 ? tp1_union(u3, v) : NULL;
	pa2m_afirmar(u4 != NULL, "… ∪ vacio valido");

	if (u4) {
		pa2m_afirmar(
			tp1_cantidad(u4) == 8,
			"Tras multiples uniones, la cantidad sigue siendo %d",
			8);

		pa2m_afirmar(tp1_buscar_id(u4, 1) != NULL, "Tiene id=1");
		pa2m_afirmar(tp1_buscar_id(u4, 2) != NULL, "Tiene id=2");
		pa2m_afirmar(tp1_buscar_id(u4, 3) != NULL, "Tiene id=3");
		pa2m_afirmar(tp1_buscar_id(u4, 4) != NULL, "Tiene id=4");
	}

	if (u4)
		tp1_destruir(u4);
	if (u3)
		tp1_destruir(u3);
	if (u2)
		tp1_destruir(u2);
	if (u1)
		tp1_destruir(u1);

	tp1_destruir(a);
	tp1_destruir(b);
	tp1_destruir(v);
}

void tp1_union_primer_parametro_null_devuelve_null()
{
	tp1_t *b = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(b != NULL, "Se crea "
				"otro_tp"
				" correctamente");
	if (!b)
		return;

	tp1_t *u = tp1_union(NULL, b);
	pa2m_afirmar(u == NULL,
		     "tp1_union con primer parametro NULL devuelve NULL");

	tp1_destruir(b);
}

void tp1_union_segundo_parametro_null_devuelve_null()
{
	tp1_t *a = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(a != NULL, "Se crea "
				"un_tp"
				" correctamente");
	if (!a)
		return;

	tp1_t *u = tp1_union(a, NULL);
	pa2m_afirmar(u == NULL,
		     "tp1_union con segundo parametro NULL devuelve NULL");

	tp1_destruir(a);
}

void tp1_union_ambos_parametros_null_devuelve_null()
{
	pa2m_afirmar(tp1_union(NULL, NULL) == NULL,
		     "tp1_union con ambos parametros NULL devuelve NULL");
}

void pruebas_tp1_union()
{
	pa2m_nuevo_grupo("TP1 union parametros validos");
	tp1_union_mismo_tp_devuelve_otro_igual();
	tp1_union_tp_distintos_devuelve_otro_con_suma_correcta();
	tp1_union_dos_tp_vacio_devuelve_tp_vacio();
	tp1_union_largo_suma_correctamente();
	tp1_union_unir_muchas_veces_sigue_contando_bien();

	pa2m_nuevo_grupo("TP1 union parametros invalidos");
	tp1_union_primer_parametro_null_devuelve_null();
	tp1_union_segundo_parametro_null_devuelve_null();
	tp1_union_ambos_parametros_null_devuelve_null();
}
//----

void tp1_interseccion_mismo_tp_devuelve_otro_igual()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(a != NULL, "Tp A creado correctamente");
	if (!a)
		return;

	size_t cant_a = tp1_cantidad(a);

	tp1_t *i = tp1_interseccion(a, a);
	pa2m_afirmar(i != NULL,
		     "Interseccion(A, A) devuelve estructura valida");
	if (i) {
		size_t cant_i = tp1_cantidad(i);
		pa2m_afirmar(
			cant_i == cant_a,
			"La cantidad en la interseccion coincide con la original (%zu)",
			cant_a);

		struct pokemon *p;
		p = tp1_buscar_id(a, 1);
		if (p)
			pa2m_afirmar(tp1_buscar_id(i, 1) != NULL,
				     "id=1 esta en la interseccion");
		p = tp1_buscar_id(a, 2);
		if (p)
			pa2m_afirmar(tp1_buscar_id(i, 2) != NULL,
				     "id=2 esta en la interseccion");
		p = tp1_buscar_id(a, 3);
		if (p)
			pa2m_afirmar(tp1_buscar_id(i, 3) != NULL,
				     "id=3 esta en la interseccion");
		p = tp1_buscar_id(a, 4);
		if (p)
			pa2m_afirmar(tp1_buscar_id(i, 4) != NULL,
				     "id=4 esta en la interseccion");
		p = tp1_buscar_id(a, 5);
		if (p)
			pa2m_afirmar(tp1_buscar_id(i, 5) != NULL,
				     "id=5 esta en la interseccion");

		tp1_destruir(i);
	}
	tp1_destruir(a);
}

void tp1_interseccion_tp_distintos_devuelve_otro_con_cantidad_correcta()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	tp1_t *b = tp1_leer_archivo(CSV_MEZCLA);
	pa2m_afirmar(a && b, "Tp A y B creados correctamente");
	if (!a || !b) {
		if (a)
			tp1_destruir(a);
		if (b)
			tp1_destruir(b);
		return;
	}

	struct ctx_inter ctx = { .b = b, .c = 0 };
	(void)tp1_con_cada_pokemon(a, contar_si_esta_en_b, &ctx);
	size_t esperado = ctx.c;

	tp1_t *i = tp1_interseccion(a, b);
	pa2m_afirmar(i != NULL,
		     "Interseccion(A, B) devuelve estructura valida");
	if (i) {
		pa2m_afirmar(tp1_cantidad(i) == esperado,
			     "Interseccion tiene %zu elementos", esperado);

		struct pokemon *pa, *pb;
		pa = tp1_buscar_id(a, 1);
		pb = tp1_buscar_id(b, 1);
		if (pa && pb)
			pa2m_afirmar(tp1_buscar_id(i, 1) != NULL,
				     "Contiene id=1");
		pa = tp1_buscar_id(a, 2);
		pb = tp1_buscar_id(b, 2);
		if (pa && pb)
			pa2m_afirmar(tp1_buscar_id(i, 2) != NULL,
				     "Contiene id=2");
		pa = tp1_buscar_id(a, 3);
		pb = tp1_buscar_id(b, 3);
		if (pa && pb)
			pa2m_afirmar(tp1_buscar_id(i, 3) != NULL,
				     "Contiene id=3");
		pa = tp1_buscar_id(a, 4);
		pb = tp1_buscar_id(b, 4);
		if (pa && pb)
			pa2m_afirmar(tp1_buscar_id(i, 4) != NULL,
				     "Contiene id=4");
		pa = tp1_buscar_id(a, 5);
		pb = tp1_buscar_id(b, 5);
		if (pa && pb)
			pa2m_afirmar(tp1_buscar_id(i, 5) != NULL,
				     "Contiene id=5");

		tp1_destruir(i);
	}

	tp1_destruir(a);
	tp1_destruir(b);
}

void tp1_interseccion_dos_tp_vacio_devuelve_tp_vacio()
{
	tp1_t *v1 = tp1_leer_archivo(CSV_VACIO);
	tp1_t *v2 = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(v1 && v2, "Tp A y B creados correctamente");

	tp1_t *i = tp1_interseccion(v1, v2);
	pa2m_afirmar(i != NULL,
		     "Interseccion de vacios devuelve estructura valida");
	if (i) {
		pa2m_afirmar(tp1_cantidad(i) == 0,
			     "Cantidad 0 en la interseccion vacia");
		tp1_destruir(i);
	}

	tp1_destruir(v1);
	tp1_destruir(v2);
}

void tp1_interseccion_largo_cantidad_correcta()
{
	tp1_t *largo = tp1_leer_archivo(CSV_LARGO);
	tp1_t *normal = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(largo && normal, "Tp A y B creados correctamente");
	if (!largo || !normal) {
		if (largo)
			tp1_destruir(largo);
		if (normal)
			tp1_destruir(normal);
		return;
	}

	struct ctx_inter ctx = { .b = normal, .c = 0 };
	(void)tp1_con_cada_pokemon(largo, contar_si_esta_en_b, &ctx);
	size_t esperado = ctx.c;

	tp1_t *i = tp1_interseccion(largo, normal);
	pa2m_afirmar(i != NULL,
		     "Interseccion(largo, normal) devuelve estructura valida");
	if (i) {
		pa2m_afirmar(
			tp1_cantidad(i) == esperado,
			"Cantidad correcta en la interseccion largo+normal (%zu)",
			esperado);

		pa2m_afirmar(tp1_buscar_id(i, 30) == NULL,
			     "No esta id=30 (solo en largo)");
		pa2m_afirmar(tp1_buscar_id(i, 2) == NULL,
			     "No esta id=2 (solo en normal)");
		pa2m_afirmar(tp1_buscar_id(i, 1) != NULL || esperado == 0,
			     "Si comparten, id=17 esta en la interseccion");

		tp1_destruir(i);
	}

	tp1_destruir(largo);
	tp1_destruir(normal);
}

void tp1_interseccion_intersecar_muchas_veces_sigue_contando_cantidad_bien()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	tp1_t *b = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	tp1_t *v = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(a && b && v, "Tp A, B y vacio creados correctamente");
	if (!a || !b || !v) {
		if (a)
			tp1_destruir(a);
		if (b)
			tp1_destruir(b);
		if (v)
			tp1_destruir(v);
		return;
	}

	struct ctx_inter ctx = { .b = b, .c = 0 };
	(void)tp1_con_cada_pokemon(a, contar_si_esta_en_b, &ctx);
	size_t esperado = ctx.c;

	tp1_t *i1 = tp1_interseccion(a, b);
	pa2m_afirmar(i1 != NULL, "Interseccion(A, B) valido");
	if (!i1) {
		tp1_destruir(a);
		tp1_destruir(b);
		tp1_destruir(v);
		return;
	}

	tp1_t *i2 = tp1_interseccion(i1, a);
	pa2m_afirmar(i2 != NULL, "(A ∩ B) ∩ A valido");

	tp1_t *i3 = i2 ? tp1_interseccion(i2, b) : NULL;
	pa2m_afirmar(i3 != NULL, "((A ∩ B) ∩ A) ∩ B valido");

	if (i3) {
		pa2m_afirmar(
			tp1_cantidad(i3) == esperado,
			"Tras multiples intersecciones, la cantidad sigue siendo %zu",
			esperado);

		if (esperado > 0) {
			struct pokemon *pa = tp1_buscar_id(a, 1),
				       *pb = tp1_buscar_id(b, 1);
			if (pa && pb)
				pa2m_afirmar(tp1_buscar_id(i3, 1) != NULL,
					     "Tiene id=1");
		}
	}

	if (i3)
		tp1_destruir(i3);
	if (i2)
		tp1_destruir(i2);
	if (i1)
		tp1_destruir(i1);
	tp1_destruir(a);
	tp1_destruir(b);
	tp1_destruir(v);
}

void tp1_interseccion_primer_parametro_null_devuelve_null()
{
	tp1_t *b = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(b != NULL, "Se crea "
				"otro_tp"
				" correctamente");
	if (!b)
		return;

	tp1_t *u = tp1_interseccion(NULL, b);
	pa2m_afirmar(
		u == NULL,
		"tp1_interseccion con primer parametro NULL devuelve NULL");

	tp1_destruir(b);
}

void tp1_interseccion_segundo_parametro_null_devuelve_null()
{
	tp1_t *a = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(a != NULL, "Se crea "
				"un_tp"
				" correctamente");
	if (!a)
		return;

	tp1_t *u = tp1_interseccion(a, NULL);
	pa2m_afirmar(
		u == NULL,
		"tp1_interseccion con segundo parametro NULL devuelve NULL");

	tp1_destruir(a);
}

void tp1_interseccion_ambos_parametros_null_devuelve_null()
{
	pa2m_afirmar(
		tp1_interseccion(NULL, NULL) == NULL,
		"tp1_interseccion con ambos parametros NULL devuelve NULL");
}

void pruebas_tp1_interseccion()
{
	pa2m_nuevo_grupo("TP1 interseccion parametros validos");
	tp1_interseccion_mismo_tp_devuelve_otro_igual();
	tp1_interseccion_tp_distintos_devuelve_otro_con_cantidad_correcta();
	tp1_interseccion_dos_tp_vacio_devuelve_tp_vacio();
	tp1_interseccion_largo_cantidad_correcta();
	tp1_interseccion_intersecar_muchas_veces_sigue_contando_cantidad_bien();

	pa2m_nuevo_grupo("TP1 interseccion parametros invalidos");
	tp1_interseccion_primer_parametro_null_devuelve_null();
	tp1_interseccion_segundo_parametro_null_devuelve_null();
	tp1_interseccion_ambos_parametros_null_devuelve_null();
}
//----

void tp1_diferencia_mismo_tp_devuelve_otro_vacio()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(a != NULL, "Tp A creado correctamente");
	if (!a)
		return;

	tp1_t *d = tp1_diferencia(a, a);
	pa2m_afirmar(d != NULL, "Diferencia(A, A) devuelve estructura valida");
	if (d) {
		pa2m_afirmar(tp1_cantidad(d) == 0,
			     "La diferencia A\\A es vacia (0)");
		tp1_destruir(d);
	}
	tp1_destruir(a);
}

void tp1_diferencia_tp_distintos_devuelve_el_mismo()
{
	tp1_t *a = tp1_leer_archivo(CSV_NORMAL);
	tp1_t *b = tp1_leer_archivo(CSV_MEZCLA);
	pa2m_afirmar(a && b, "Tp A y B creados correctamente");
	if (!a || !b) {
		if (a)
			tp1_destruir(a);
		if (b)
			tp1_destruir(b);
		return;
	}

	struct ctx_diff ctx = { .b = b, .c = 0 };
	(void)tp1_con_cada_pokemon(a, contar_si_no_esta_en_b, &ctx);
	size_t esperado = ctx.c;

	tp1_t *d = tp1_diferencia(a, b);
	pa2m_afirmar(d != NULL, "Diferencia(A, B) devuelve estructura valida");
	if (d) {
		pa2m_afirmar(tp1_cantidad(d) == esperado,
			     "Diferencia tiene %zu elementos", esperado);

		for (int id = 1; id <= 5; ++id) {
			struct pokemon *pa = tp1_buscar_id(a, id);
			struct pokemon *pb = tp1_buscar_id(b, id);
			if (pa && !pb)
				pa2m_afirmar(tp1_buscar_id(d, id) != NULL,
					     "Contiene id=%d (exclusivo de A)",
					     id);
		}

		tp1_destruir(d);
	}

	tp1_destruir(a);
	tp1_destruir(b);
}

void tp1_diferencia_dos_tp_vacio_devuelve_tp_vacio()
{
	tp1_t *v1 = tp1_leer_archivo(CSV_VACIO);
	tp1_t *v2 = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(v1 && v2, "Tp A y B creados correctamente");

	tp1_t *d = tp1_diferencia(v1, v2);
	pa2m_afirmar(d != NULL,
		     "Diferencia de vacios devuelve estructura valida");
	if (d) {
		pa2m_afirmar(tp1_cantidad(d) == 0,
			     "Cantidad 0 en la diferencia vacia");
		tp1_destruir(d);
	}

	tp1_destruir(v1);
	tp1_destruir(v2);
}

void tp1_diferencia_largo_cantidad_correcta()
{
	tp1_t *largo = tp1_leer_archivo(CSV_LARGO);
	tp1_t *normal = tp1_leer_archivo(CSV_NORMAL);
	pa2m_afirmar(largo && normal, "Tp A y B creados correctamente");
	if (!largo || !normal) {
		if (largo)
			tp1_destruir(largo);
		if (normal)
			tp1_destruir(normal);
		return;
	}

	struct ctx_diff ctx = { .b = normal, .c = 0 };
	(void)tp1_con_cada_pokemon(largo, contar_si_no_esta_en_b, &ctx);
	size_t esperado = ctx.c;

	tp1_t *d = tp1_diferencia(largo, normal);
	pa2m_afirmar(d != NULL,
		     "Diferencia(largo, normal) devuelve estructura valida");
	if (d) {
		pa2m_afirmar(
			tp1_cantidad(d) == esperado,
			"Cantidad correcta en la diferencia largo+normal (%zu)",
			esperado);

		pa2m_afirmar(
			tp1_buscar_id(d, 30) != NULL || esperado == 0,
			"Si es exclusivo de A, id=30 esta en la diferencia");
		pa2m_afirmar(tp1_buscar_id(d, 2) == NULL,
			     "No esta id=2 (solo en B)");
		pa2m_afirmar(tp1_buscar_id(d, 1) == NULL,
			     "No esta id=1 (esta en ambos)");

		tp1_destruir(d);
	}

	tp1_destruir(largo);
	tp1_destruir(normal);
}

void tp1_diferencia_primer_parametro_null_devuelve_null()
{
	tp1_t *b = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(b != NULL, "Se crea "
				"otro_tp"
				" correctamente");
	if (!b)
		return;

	tp1_t *u = tp1_diferencia(NULL, b);
	pa2m_afirmar(u == NULL,
		     "tp1_diferencia con primer parametro NULL devuelve NULL");

	tp1_destruir(b);
}

void tp1_diferencia_segundo_parametro_null_devuelve_null()
{
	tp1_t *a = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(a != NULL, "Se crea "
				"un_tp"
				" correctamente");
	if (!a)
		return;

	tp1_t *u = tp1_diferencia(a, NULL);
	pa2m_afirmar(u == NULL,
		     "tp1_diferencia con segundo parametro NULL devuelve NULL");

	tp1_destruir(a);
}

void tp1_diferencia_ambos_parametros_null_devuelve_null()
{
	pa2m_afirmar(tp1_diferencia(NULL, NULL) == NULL,
		     "tp1_diferencia con ambos parametros NULL devuelve NULL");
}

void pruebas_tp1_diferencia()
{
	pa2m_nuevo_grupo("TP1 diferencia parametros validos");
	tp1_diferencia_mismo_tp_devuelve_otro_vacio();
	tp1_diferencia_tp_distintos_devuelve_el_mismo();
	tp1_diferencia_dos_tp_vacio_devuelve_tp_vacio();
	tp1_diferencia_largo_cantidad_correcta();

	pa2m_nuevo_grupo("TP1 diferencia parametros invalidos");
	tp1_diferencia_primer_parametro_null_devuelve_null();
	tp1_diferencia_segundo_parametro_null_devuelve_null();
	tp1_diferencia_ambos_parametros_null_devuelve_null();
}
//----

void tp1_buscar_nombre_nombre_correcto_encuentra_pokemon()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_nombre(tp, "Meowth");
	pa2m_afirmar(p != NULL,
		     "tp1_buscar_nombre encuentra pokemon correctamente");
	if (p)
		pa2m_afirmar(
			p->id == 52,
			"El id del pokemon encontrado es el esperado (52)");

	tp1_destruir(tp);
}

void tp1_buscar_nombre_en_archivo_largo_encuentra_nombre_largo()
{
	tp1_t *tp = tp1_leer_archivo(CSV_LARGO);
	pa2m_afirmar(tp != NULL, "Tp largo creado correctamente");
	if (!tp)
		return;

	struct pokemon *ref = tp1_buscar_id(tp, 1);
	pa2m_afirmar(ref != NULL, "Referencia por id existente obtenida");
	if (!ref) {
		tp1_destruir(tp);
		return;
	}

	struct pokemon *p = tp1_buscar_nombre(tp, ref->nombre);
	pa2m_afirmar(
		p != NULL,
		"tp1_buscar_nombre busca por nombre largo encuentra pokemon correctamente");
	if (p)
		pa2m_afirmar(p->id == ref->id,
			     "El id coincide con la referencia");

	tp1_destruir(tp);
}

void tp1_buscar_nombre_no_altera_cantidad()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	size_t antes = tp1_cantidad(tp);
	(void)tp1_buscar_nombre(tp, "Pikachu");
	(void)tp1_buscar_nombre(tp, "NoExiste");
	size_t despues = tp1_cantidad(tp);

	pa2m_afirmar(antes == despues,
		     "Buscar por nombre no altera la cantidad de pokemones");

	tp1_destruir(tp);
}

void tp1_buscar_nombre_tp_null_devuelve_null()
{
	struct pokemon *p = tp1_buscar_nombre(NULL, "Pikachu");
	pa2m_afirmar(p == NULL, "tp1_buscar_nombre con tp NULL devuelve NULL");
}

void tp1_buscar_nombre_nombre_null_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_nombre(tp, NULL);
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_nombre con nombre NULL devuelve NULL");

	tp1_destruir(tp);
}

void tp1_buscar_nombre_tp_vacio_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL, "Tp vacio creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_nombre(tp, "Cualquiera");
	pa2m_afirmar(p == NULL, "tp1_buscar_nombre con tp vacio devuelve NULL");

	tp1_destruir(tp);
}

void pruebas_tp1_buscar_nombre()
{
	pa2m_nuevo_grupo("TP1 buscar por nombre parametros validos");
	tp1_buscar_nombre_nombre_correcto_encuentra_pokemon();
	tp1_buscar_nombre_en_archivo_largo_encuentra_nombre_largo();
	tp1_buscar_nombre_no_altera_cantidad();

	pa2m_nuevo_grupo("TP1 buscar por nombre parametros invalidos");
	tp1_buscar_nombre_tp_null_devuelve_null();
	tp1_buscar_nombre_nombre_null_devuelve_null();
	tp1_buscar_nombre_tp_vacio_devuelve_null();
}
//----

void tp1_buscar_id_id_correcto_encuentra_pokemon()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_id(tp, 52);
	pa2m_afirmar(p != NULL,
		     "tp1_buscar_id encuentra pokemon correctamente");
	if (p)
		pa2m_afirmar(
			strcmp(p->nombre, "Meowth") == 0,
			"El nombre del pokemon encontrado es el esperado (Meowth)");

	tp1_destruir(tp);
}

void tp1_buscar_id_en_archivo_largo_encuentra_pokemon()
{
	tp1_t *tp = tp1_leer_archivo(CSV_LARGO);
	pa2m_afirmar(tp != NULL, "Tp largo creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_id(tp, 81);
	pa2m_afirmar(p != NULL,
		     "tp1_buscar_id encuentra pokemon en archivo largo");

	tp1_destruir(tp);
}

void tp1_buscar_id_no_altera_cantidad()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	size_t antes = tp1_cantidad(tp);
	(void)tp1_buscar_id(tp, 25);
	(void)tp1_buscar_id(tp, 9999);
	size_t despues = tp1_cantidad(tp);

	pa2m_afirmar(antes == despues,
		     "Buscar por id no altera la cantidad de pokemones");

	tp1_destruir(tp);
}

void tp1_buscar_id_tp_null_devuelve_null()
{
	struct pokemon *p = tp1_buscar_id(NULL, 25);
	pa2m_afirmar(p == NULL, "tp1_buscar_id con tp NULL devuelve NULL");
}

void tp1_buscar_id_tp_vacio_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL, "Tp vacio creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_id(tp, 1234);
	pa2m_afirmar(p == NULL, "tp1_buscar_id con tp vacio devuelve NULL");

	tp1_destruir(tp);
}

void tp1_buscar_id_inexistente_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_id(tp, 9999);
	pa2m_afirmar(p == NULL, "tp1_buscar_id con id invalida devuelve NULL");

	tp1_destruir(tp);
}

void tp1_buscar_id_id_negativo_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	struct pokemon *p = tp1_buscar_id(tp, -5);
	pa2m_afirmar(p == NULL, "tp1_buscar_id con id negativa devuelve NULL");

	tp1_destruir(tp);
}

void pruebas_tp1_buscar_id()
{
	pa2m_nuevo_grupo("TP1 buscar por id parametros validos");
	tp1_buscar_id_id_correcto_encuentra_pokemon();
	tp1_buscar_id_en_archivo_largo_encuentra_pokemon();
	tp1_buscar_id_no_altera_cantidad();

	pa2m_nuevo_grupo("TP1 buscar por id parametros invalidos");
	tp1_buscar_id_tp_null_devuelve_null();
	tp1_buscar_id_tp_vacio_devuelve_null();
	tp1_buscar_id_inexistente_devuelve_null();
	tp1_buscar_id_id_negativo_devuelve_null();
}
//----

void tp1_cada_pokemon_recorre_en_orden_por_id()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	size_t cant = tp1_cantidad(tp);
	int ids[1024] = { 0 };
	struct recolector_ids r = {
		.ids = ids, .cantidad_ids = 0, .capacidad = 1024, .cortar_en = 0
	};

	size_t aplicados = tp1_con_cada_pokemon(tp, recolectar_ids, &r);
	pa2m_afirmar(aplicados == cant,
		     "Aplica la funcion a todos los pokemones");
	pa2m_afirmar(estan_ordenados_por_id(ids, r.cantidad_ids),
		     "Los pokemones se recorren en orden ascendente por id");

	tp1_destruir(tp);
}

void tp1_cada_pokemon_corte_temprano_funcion_false()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	int ids[8] = { 0 };
	struct recolector_ids r = {
		.ids = ids, .cantidad_ids = 0, .capacidad = 8, .cortar_en = 2
	};

	size_t aplicados = tp1_con_cada_pokemon(tp, recolectar_ids, &r);
	pa2m_afirmar(aplicados == 2,
		     "La funcion corta temprano cuando devuelve false");
	pa2m_afirmar(estan_ordenados_por_id(ids, r.cantidad_ids),
		     "Aun con corte, el orden de ids es ascendente");

	tp1_destruir(tp);
}

void tp1_cada_pokemon_cuenta_aplicados_igual_cantidad()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	int iterados = 0;
	size_t aplicados = tp1_con_cada_pokemon(tp, contar_pokemons, &iterados);
	size_t cant = tp1_cantidad(tp);

	pa2m_afirmar(
		aplicados == cant && iterados == cant,
		"La cantidad de aplicaciones coincide con la iterada total");

	tp1_destruir(tp);
}

void tp1_cada_pokemon_tp_null_devuelve_cero()
{
	int iterados = 0;
	size_t aplicados =
		tp1_con_cada_pokemon(NULL, contar_pokemons, &iterados);
	pa2m_afirmar(aplicados == 0,
		     "tp1_cada_pokemon con tp NULL devuelve cero");
}

void tp1_cada_pokemon_funcion_null_devuelve_cero()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	size_t aplicados = tp1_con_cada_pokemon(tp, NULL, NULL);
	pa2m_afirmar(aplicados == 0,
		     "tp1_cada_pokemon con funcion NULL devuelve cero");

	tp1_destruir(tp);
}

void tp1_cada_pokemon_tp_vacio_devuelve_cero()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL, "Tp vacio creado correctamente");
	if (!tp)
		return;

	int iterados = 0;
	size_t aplicados = tp1_con_cada_pokemon(tp, contar_pokemons, &iterados);
	pa2m_afirmar(
		aplicados == 0 && iterados == 0,
		"tp1_cada_pokemon devuelve cero y coincide con cantidad iterados");

	tp1_destruir(tp);
}

void pruebas_tp1_cada_pokemon()
{
	pa2m_nuevo_grupo("TP1 con_cada_pokemon parametros validos");
	tp1_cada_pokemon_recorre_en_orden_por_id();
	tp1_cada_pokemon_corte_temprano_funcion_false();
	tp1_cada_pokemon_cuenta_aplicados_igual_cantidad();

	pa2m_nuevo_grupo("TP1 con_cada_pokemon parametros invalidos");
	tp1_cada_pokemon_tp_null_devuelve_cero();
	tp1_cada_pokemon_funcion_null_devuelve_cero();
	tp1_cada_pokemon_tp_vacio_devuelve_cero();
}
//----

void tp1_destruir_tp_valido_destruye_correctamente()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp creado correctamente");
	if (!tp)
		return;

	tp1_destruir(tp);
	pa2m_afirmar(true, "tp1_destruir(tp valido) libera correctamente");
}

void tp1_destruir_tp_con_muchos_pokemones_destruye_correctamente()
{
	tp1_t *tp = tp1_leer_archivo(CSV_LARGO);
	pa2m_afirmar(tp != NULL, "Tp largo creado correctamente");
	if (!tp)
		return;

	tp1_destruir(tp);
	pa2m_afirmar(true, "tp1_destruir(tp largo) libera correctamente");
}

void tp1_destruir_null_destruye_correctamente()
{
	tp1_destruir(NULL);
	pa2m_afirmar(true, "tp1_destruir(NULL) se borra correctamente");
}

void tp1_destruir_tp_vacio_destruye_correctamente()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL, "Tp vacio creado correctamente");
	if (!tp)
		return;

	tp1_destruir(tp);
	pa2m_afirmar(true, "tp1_destruir(tp vacio) libera correctamente");
}

void pruebas_tp1_destruir()
{
	pa2m_nuevo_grupo("TP1 destruir parametros validos");
	tp1_destruir_tp_valido_destruye_correctamente();
	tp1_destruir_tp_con_muchos_pokemones_destruye_correctamente();

	pa2m_nuevo_grupo("TP1 destruir parametros invalidos");
	tp1_destruir_tp_vacio_destruye_correctamente();
	tp1_destruir_null_destruye_correctamente();
}
//----

void tp1_guardar_archivo_sobrescribe_existente()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp leido correctamente");
	if (!tp)
		return;

	pa2m_afirmar(tp1_guardar_archivo(tp, OUT_GUARDAR_OVERWRITE) == tp,
		     "Primera escritura exitosa");
	pa2m_afirmar(tp1_guardar_archivo(tp, OUT_GUARDAR_OVERWRITE) == tp,
		     "Segunda escritura (sobrescribe) exitosa");

	tp1_t *re = tp1_leer_archivo(OUT_GUARDAR_OVERWRITE);
	pa2m_afirmar(re != NULL, "Se puede releer el archivo sobrescrito");
	if (re) {
		pa2m_afirmar(tp1_cantidad(re) == tp1_cantidad(tp),
			     "Cantidad coincide tras sobrescribir");
		tp1_destruir(re);
	}

	tp1_destruir(tp);
}

void tp1_guardar_archivo_tp_vacio_y_releer()
{
	tp1_t *tp = tp1_leer_archivo(CSV_VACIO);
	pa2m_afirmar(tp != NULL, "Tp vacio leido correctamente");
	if (!tp)
		return;

	pa2m_afirmar(tp1_guardar_archivo(tp, OUT_GUARDAR_VACIO) == tp,
		     "Guardar tp vacio devuelve el mismo puntero");

	tp1_t *re = tp1_leer_archivo(OUT_GUARDAR_VACIO);
	pa2m_afirmar(re != NULL, "Se puede releer un archivo vacio guardado");
	if (re) {
		pa2m_afirmar(tp1_cantidad(re) == 0,
			     "Cantidad 0 tras releer el archivo vacio");
		tp1_destruir(re);
	}

	tp1_destruir(tp);
}

void tp1_guardar_archivo_ruta_null_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp leido correctamente");
	if (!tp)
		return;

	pa2m_afirmar(tp1_guardar_archivo(tp, NULL) == NULL,
		     "tp1_guardar_archivo con ruta NULL devuelve NULL");
	tp1_destruir(tp);
}

void tp1_guardar_archivo_tp_null_devuelve_null()
{
	pa2m_afirmar(tp1_guardar_archivo(NULL, OUT_GUARDAR_OK) == NULL,
		     "tp1_guardar_archivo con tp NULL devuelve NULL");
}

void tp1_guardar_archivo_directorio_inexistente_devuelve_null()
{
	tp1_t *tp = tp1_leer_archivo(CSV_NOMBRE_VALIDO);
	pa2m_afirmar(tp != NULL, "Tp leido correctamente");
	if (!tp)
		return;

	pa2m_afirmar(
		tp1_guardar_archivo(tp, OUT_NOEXISTE) == NULL,
		"tp1_guardar_archivo con directorio inexistente devuelve NULL");
	tp1_destruir(tp);
}

void pruebas_tp1_guardar_archivo()
{
	pa2m_nuevo_grupo("TP1 guardar_archivo parametros validos");
	tp1_guardar_archivo_sobrescribe_existente();
	tp1_guardar_archivo_tp_vacio_y_releer();

	pa2m_nuevo_grupo("TP1 guardar_archivo parametros invalidos");
	tp1_guardar_archivo_ruta_null_devuelve_null();
	tp1_guardar_archivo_tp_null_devuelve_null();
	tp1_guardar_archivo_directorio_inexistente_devuelve_null();
}

void correr_pruebas_tp1()
{
	pruebas_tp1_leer_archivo();
	pruebas_tp1_cantidad();
	pruebas_tp1_guardar_archivo();
	pruebas_tp1_union();
	pruebas_tp1_interseccion();
	pruebas_tp1_diferencia();
	pruebas_tp1_buscar_nombre();
	pruebas_tp1_buscar_id();
	pruebas_tp1_cada_pokemon();
	pruebas_tp1_destruir();
}

int main()
{
	pa2m_nuevo_grupo("============== Pruebas TP1 ===============");
	correr_pruebas_tp1();

	return pa2m_mostrar_reporte();
}
