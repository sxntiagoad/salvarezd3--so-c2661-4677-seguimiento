.PHONY: clean all

CC=gcc
CFLAGS=-Wall
LDLIBS=

EXECS=jardin_ornamental.exe jardin_ornamental_limitado.exe jardin_ornamental_general.exe \
      agente.exe fumador.exe ctrl_fumadores.exe \
      agente2.exe fumadores2.exe ctrl_fumadores2.exe

all: ${EXECS}

# --- Jardin sin limite ---
jardin_ornamental.exe: jardin_ornamental.o jardin_utilidades.o
	${CC} -o $@ $^ ${LDLIBS}
jardin_ornamental.o: jardin_ornamental.c jardin_ornamental.h
	${CC} ${CFLAGS} -c $<
jardin_utilidades.o: jardin_utilidades.c jardin_ornamental.h
	${CC} ${CFLAGS} -c $<

# --- Jardin limitado ---
jardin_ornamental_limitado.exe: jardin_ornamental_limitado.o jardin_limitado_utilidades.o
	${CC} -o $@ $^ ${LDLIBS}
jardin_ornamental_limitado.o: jardin_ornamental_limitado.c jardin_ornamental_limitado.h
	${CC} ${CFLAGS} -c $<
jardin_limitado_utilidades.o: jardin_limitado_utilidades.c jardin_ornamental_limitado.h
	${CC} ${CFLAGS} -c $<

# --- Ejercicio 4: jardin general ---
jardin_ornamental_general.exe: jardin_ornamental_general.o jardin_ornamental_utilidades_general.o
	${CC} -o $@ $^ ${LDLIBS}
jardin_ornamental_general.o: jardin_ornamental_general.c jardin_ornamental_general.h
	${CC} ${CFLAGS} -c $<
jardin_ornamental_utilidades_general.o: jardin_ornamental_utilidades_general.c jardin_ornamental_general.h
	${CC} ${CFLAGS} -c $<

# --- Fumadores originales ---
agente.exe: agente.o fumadores_utilidad.o
	${CC} -o $@ $^
agente.o: agente.c fumadores.h
	${CC} ${CFLAGS} -c $<
fumador.exe: fumador.o fumadores_utilidad.o
	${CC} -o $@ $^
fumador.o: fumador.c fumadores.h
	${CC} ${CFLAGS} -c $<
ctrl_fumadores.exe: ctrl_fumadores.o fumadores_utilidad.o
	${CC} -o $@ $^
ctrl_fumadores.o: ctrl_fumadores.c fumadores.h
	${CC} ${CFLAGS} -c $<
fumadores_utilidad.o: fumadores_utilidad.c fumadores.h
	${CC} ${CFLAGS} -c $<

# --- Ejercicio 6: fumadores refactorizados ---
agente2.exe: agente2.o fumadores_utilidad2.o
	${CC} -o $@ $^
agente2.o: agente2.c fumadores2.h
	${CC} ${CFLAGS} -c $<
fumadores2.exe: fumadores2.o fumadores_utilidad2.o
	${CC} -o $@ $^
fumadores2.o: fumadores2.c fumadores2.h
	${CC} ${CFLAGS} -c $<
ctrl_fumadores2.exe: ctrl_fumadores2.o fumadores_utilidad2.o
	${CC} -o $@ $^
ctrl_fumadores2.o: ctrl_fumadores2.c fumadores2.h
	${CC} ${CFLAGS} -c $<
fumadores_utilidad2.o: fumadores_utilidad2.c fumadores2.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o
	rm -f ${EXECS}
	rm -f *~
