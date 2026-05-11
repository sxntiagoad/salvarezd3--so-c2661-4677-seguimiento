#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "fumadores2.h"

/* Un unico puntero estatico aqui dentro para que el handler de
   senal pueda liberar recursos (sigaction no permite pasar contexto). */
static PEstadoAgente g_estado_handler = NULL;

static void manejador(int signo) {
    (void) signo;
    destruir_estado_agente(g_estado_handler);
    _exit(EXIT_SUCCESS);
}

static void usage(char const *prog, int code) {
    fprintf(stderr,
            "Uso: %s [-a <nom>] [-c <nom>] [-p <nom>] [-t <nom>]\n"
            "     %s -h\n", prog, prog);
    exit(code);
}

static void ejecutar_agente(PEstadoAgente e) {
    for (;;) {
        SEM_ID s = rand() % AGENTE;
        sleep(rand() % MAX_TIEMPO_PREPARANDO + 1);

        fprintf(stdout, "[Agente] ");
        switch (s) {
            case CERILLA: fprintf(stdout, "poniendo papel y tabaco\n"); break;
            case PAPEL:   fprintf(stdout, "poniendo cerilla y tabaco\n"); break;
            case TABACO:  fprintf(stdout, "poniendo cerilla y papel\n"); break;
            default: break;
        }

        sem_post(e->sems[s]);
        fprintf(stdout, "[Agente] Esperando continuar\n");
        sem_wait(e->sems[AGENTE]);
    }
}

int main(int argc, char *argv[]) {
    char const *nom_sems[NUM_SEMS] = {
        NOM_SEM_CERILLA, NOM_SEM_PAPEL, NOM_SEM_TABACO, NOM_SEM_AGENTE
    };
    int opt;

    while ((opt = getopt(argc, argv, "a:c:hp:t:")) != -1) {
        switch (opt) {
            case 'a': nom_sems[AGENTE]  = optarg; break;
            case 'c': nom_sems[CERILLA] = optarg; break;
            case 'p': nom_sems[PAPEL]   = optarg; break;
            case 't': nom_sems[TABACO]  = optarg; break;
            case 'h': usage(argv[0], EXIT_SUCCESS); break;
            default:  usage(argv[0], EXIT_FAILURE);
        }
    }

    srand(time(NULL));

    PEstadoAgente estado = crear_estado_agente(nom_sems);
    if (estado == NULL) exit(EXIT_FAILURE);

    g_estado_handler = estado;

    struct sigaction sa;
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    ejecutar_agente(estado);
    return EXIT_SUCCESS;
}
