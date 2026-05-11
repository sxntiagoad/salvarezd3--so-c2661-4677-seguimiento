#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "fumadores2.h"

static PEstadoAgente g_estado_handler = NULL;

static BOOL WINAPI handle_signals(DWORD dwCtrlType) {
    (void) dwCtrlType;
    destruir_estado_agente(g_estado_handler);
    ExitProcess(EXIT_SUCCESS);
    return TRUE;
}

static void usage(char const *prog, DWORD code) {
    fprintf(stderr,
            "Uso: %s [-a <nom>] [-c <nom>] [-p <nom>] [-t <nom>]\r\n"
            "     %s -h\r\n", prog, prog);
    ExitProcess(code);
}

static void ejecutar_agente(PEstadoAgente e) {
    for (;;) {
        SEM_ID s = rand() % AGENTE;
        Sleep(rand() % MAX_TIEMPO_PREPARANDO + 1);

        fprintf(stdout, "[Agente] ");
        switch (s) {
            case CERILLA: fprintf(stdout, "poniendo papel y tabaco\r\n"); break;
            case PAPEL:   fprintf(stdout, "poniendo cerilla y tabaco\r\n"); break;
            case TABACO:  fprintf(stdout, "poniendo cerilla y papel\r\n"); break;
            default: break;
        }

        ReleaseSemaphore(e->hSems[s], 1, NULL);
        fprintf(stdout, "[Agente] Esperando continuar\r\n");
        WaitForSingleObject(e->hSems[AGENTE], INFINITE);
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
    if (estado == NULL) ExitProcess(EXIT_FAILURE);

    g_estado_handler = estado;
    SetConsoleCtrlHandler(handle_signals, TRUE);

    ejecutar_agente(estado);
    ExitProcess(EXIT_SUCCESS);
}
