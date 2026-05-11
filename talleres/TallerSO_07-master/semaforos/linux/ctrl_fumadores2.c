#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "fumadores2.h"

static PEstadoControl g_control = NULL;

static void manejador(int signo) {
    (void) signo;
    if (g_control == NULL) return;
    for (int i = 0; i < g_control->num_procesos; i++) {
        if (g_control->procesos[i] > 0) kill(g_control->procesos[i], SIGINT);
    }
}

static void usage(char const *prog, int code) {
    fprintf(stderr,
            "Uso: %s [-a <nom>] [-c <nom>] [-p <nom>] [-t <nom>]\n"
            "     %s -h\n", prog, prog);
    exit(code);
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

    PEstadoControl control = crear_estado_control(NUM_SEMS);
    if (control == NULL) exit(EXIT_FAILURE);
    g_control = control;

    /* Lanza el agente */
    control->procesos[0] = fork();
    if (control->procesos[0] == 0) {
        execl("./agente2", "agente2",
              "-a", nom_sems[AGENTE],
              "-c", nom_sems[CERILLA],
              "-p", nom_sems[PAPEL],
              "-t", nom_sems[TABACO],
              NULL);
        _exit(EXIT_FAILURE);
    }

    sleep(MAX_TIEMPO_PREPARANDO);

    /* Lanza los tres fumadores */
    for (int i = 0; i < AGENTE; i++) {
        control->procesos[i+1] = fork();
        if (control->procesos[i+1] == 0) {
            execl("./fumadores2", "fumadores2",
                  "-a", nom_sems[AGENTE],
                  "-f", nom_sems[i],
                  NULL);
            _exit(EXIT_FAILURE);
        }
    }

    struct sigaction sa;
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    int estado;
    for (int i = 0; i < control->num_procesos; i++) {
        waitpid(control->procesos[i], &estado, 0);
    }

    destruir_estado_control(control);
    return EXIT_SUCCESS;
}
