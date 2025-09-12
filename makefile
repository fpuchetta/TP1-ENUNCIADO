CFLAGS = -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g -Isrc
SRC = src/*.c

tp1: $(SRC) main.c
	gcc $(CFLAGS) $^ -o $@

pruebas: $(SRC) pruebas_alumno.c
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f abb