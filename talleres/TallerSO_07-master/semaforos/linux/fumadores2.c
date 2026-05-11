/* Proceso fumador (refactor sin variables globales). */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "fumadores2.h"

#define SEM_AGENTE_IDX  0
#define SEM_FUMADOR_IDX 1

struct EstadoFumador {
    sem_t *sems[2];
    char  *id_fumador;
};
typedef struct EstadoFumador EstadoFumador;
typedef struct EstadoFumador *PEstadoFumador;

static PEstadoFumador g_estado = NULL;

static void destruir_estado_fumador(PEstadoFumador e) {
    if (e == NULL) return;
    if (e->sems[0]) sem_close(e->sems[0]);
    if (e->sems[1]) sem_close(e->sems[1]);
    free(e->id_fumador);
    free(e);
}

static void manejador(int signo) {
    (void) signo;
    destruir_estado_fumador(g_estado);
    _exit(EXIT_SUCCESS);
}

static void usage(char const *prog, int code) {
    fprintf(stderr, "Uso: %s [-a <nom>] -f <nom>\n     %s -h\n", prog, prog);
    exit(code);
}

static PEstadoFumador crear_estado_fumador(char const *nom_agente,
                                           char const *nom_fumador,
                                           char const *id_fumador) {
    PEstadoFumador e = malloc(sizeof(EstadoFumador));
    if (e == NULL) return NULL;
    e->sems[0] = e->sems[1] = NULL;
    e->id_fumador = strdup(id_fumador);

    char const *nombres[2] = { nom_agente, nom_fumador };
    for (int i = 0; i < 2; i++) {
        char *real = obtener_nombre_semaforo(PRE_GLOBAL, nombres[i]);
        fprintf(stdout, "[fumador %s] abriendo semaforo %s\n", id_fumador, real);
        e->sems[i] = sem_open(real, 0);
        if (e->sems[i] == SEM_FAILED) {
            fprintf(stderr, "Error abriendo semaforo %s (%d)\n", real, errno);
            free(real);
            destruir_estado_fumador(e);
            return NULL;
        }
        free(real);
    }
    return e;
}

static void ejecutar_fumador(PEstadoFumador e) {
    for (;;) {
        fprintf(stdout, "[Fumador %s] esperando\n", e->id_fumador);
        sem_wait(e->sems[SEM_FUMADOR_IDX]);
        fprintf(stdout, "[Fumador %s] tomo ingredientes, lia y fuma\n", e->id_fumador);
        sleep(rand() % MAX_TIEMPO_FUMANDO + 1);
        sem_post(e->sems[SEM_AGENTE_IDX]);
    }
}

int main(int argc, char *argv[]) {
    char const *nom_agente  = NOM_SEM_AGENTE;
    char const *nom_fumador = NULL;
    char const *id_fumador  = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "a:f:h")) != -1) {
        switch (opt) {
            case 'a': nom_agente  = optarg; break;
            case 'f': nom_fumador = optarg; id_fumador = optarg; break;
            case 'h': usage(argv[0], EXIT_SUCCESS); break;
            default:  usage(argv[0], EXIT_FAILURE);
        }
    }
    if (nom_fumador == NULL) usage(argv[0], EXIT_FAILURE);

    srand(time(NULL) ^ getpid());

    PEstadoFumador estado = crear_estado_fumador(nom_agente, nom_fumador, id_fumador);
    if (estado == NULL) exit(EXIT_FAILURE);
    g_estado = estado;

    struct sigaction sa;
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    ejecutar_fumador(estado);
    return EXIT_SUCCESS;
}
