#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "jardin_ornamental_general.h"

static int random_siguiente(int max, int min) {
    double r = (double) rand();
    double u = (double) RAND_MAX;
    return (int)((r / u) * (max - min)) + min;
}

static void usage(char const *progname, int codigo_salida) {
    fprintf(stderr,
            "Uso: %s [-l <limite>] -m <maximo>\n"
            "     %s -m <maximo>\n"
            "     %s -h\n",
            progname, progname, progname);
    exit(codigo_salida);
}

static void* visitante(void *arg) {
    PJardin_General jardin = (PJardin_General) arg;
    pthread_t hilo = pthread_self();

    for (int v = 0; v < MAX_VISITAS; v++) {
        fprintf(stdout, "Visitante %ld afuera\n", hilo);
        sleep(random_siguiente(TIEMPO_AFUERA, 0));

        entrar_jardin(jardin);
        fprintf(stdout, "Visitante %ld dentro del jardin\n", hilo);

        sleep(random_siguiente(TIEMPO_VISITA, 0));
        salir_jardin(jardin);
    }
    fprintf(stdout, "Visitante %ld termino su jornada\n", hilo);
    return NULL;
}

static void* admin(void *arg) {
    PJardin_General jardin = (PJardin_General) arg;
    for (int v = 0; v < MAX_VISITAS; v++) {
        sleep(random_siguiente(TIEMPO_AFUERA * 2, 0));
        fprintf(stdout, "Visitantes %d dentro del jardin\n",
                obtener_miembros_jardin(jardin));
    }
    fprintf(stdout, "Administrador termino su jornada\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    int limite = 0;       /* 0 = sin limite */
    int maximo = 0;
    int opt;

    while ((opt = getopt(argc, argv, "l:m:h")) != -1) {
        switch (opt) {
            case 'l': limite = atoi(optarg); break;
            case 'm': maximo = atoi(optarg); break;
            case 'h': usage(argv[0], EXIT_SUCCESS); break;
            default:  usage(argv[0], EXIT_FAILURE);
        }
    }

    if (maximo <= 0) {
        fprintf(stderr, "Error: -m <maximo> es obligatorio y > 0\n");
        usage(argv[0], EXIT_FAILURE);
    }
    if (limite < 0 || (limite > 0 && limite >= maximo)) {
        fprintf(stderr, "Error: se requiere 0 < limite < maximo\n");
        exit(EXIT_FAILURE);
    }

    PJardin_General jardin = obtener_jardin(limite);
    if (jardin == NULL) {
        fprintf(stderr, "Error: no se pudo crear el jardin\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Jardin %s (maximo=%d, limite=%d)\n",
            (limite > 0 ? "LIMITADO" : "SIN LIMITE"), maximo, limite);
    fprintf(stdout, "Visitantes antes de iniciar: %d\n",
            obtener_miembros_jardin(jardin));

    pthread_t *hilos = malloc(sizeof(pthread_t) * (maximo + 1));
    srand(time(NULL));

    for (int i = 0; i < maximo; i++) {
        if (pthread_create(&hilos[i], NULL, visitante, jardin) != 0) {
            fprintf(stderr, "Error creando hilo %d: %d\n", i, errno);
            exit(EXIT_FAILURE);
        }
    }
    pthread_create(&hilos[maximo], NULL, admin, jardin);

    for (int i = 0; i < maximo + 1; i++) pthread_join(hilos[i], NULL);

    fprintf(stdout, "Visitantes antes de cerrar: %d\n",
            obtener_miembros_jardin(jardin));

    cerrar_jardin(jardin);
    free(hilos);
    return EXIT_SUCCESS;
}
