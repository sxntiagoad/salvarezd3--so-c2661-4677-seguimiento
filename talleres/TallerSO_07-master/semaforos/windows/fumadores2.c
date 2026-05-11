/* Proceso fumador (refactor Windows sin variables globales). */
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "fumadores2.h"

#define SEM_AGENTE_IDX  0
#define SEM_FUMADOR_IDX 1

struct EstadoFumador {
    HANDLE hSems[2];
    char  *id_fumador;
};
typedef struct EstadoFumador EstadoFumador;
typedef struct EstadoFumador *PEstadoFumador;

static PEstadoFumador g_estado = NULL;

static void destruir_estado_fumador(PEstadoFumador e) {
    if (e == NULL) return;
    if (e->hSems[0]) CloseHandle(e->hSems[0]);
    if (e->hSems[1]) CloseHandle(e->hSems[1]);
    free(e->id_fumador);
    free(e);
}

static BOOL WINAPI handle_signals(DWORD dwCtrlType) {
    (void) dwCtrlType;
    destruir_estado_fumador(g_estado);
    ExitProcess(EXIT_SUCCESS);
    return TRUE;
}

static void usage(char const *prog, DWORD code) {
    fprintf(stderr, "Uso: %s [-a <nom>] -f <nom>\r\n     %s -h\r\n", prog, prog);
    ExitProcess(code);
}

static PEstadoFumador crear_estado_fumador(char const *nom_agente,
                                           char const *nom_fumador,
                                           char const *id_fumador) {
    PEstadoFumador e = malloc(sizeof(EstadoFumador));
    if (e == NULL) return NULL;
    ZeroMemory(e, sizeof(EstadoFumador));
    e->id_fumador = _strdup(id_fumador);

    char const *nombres[2] = { nom_agente, nom_fumador };
    for (int i = 0; i < 2; i++) {
        char *real = obtener_nombre_semaforo(PRE_GLOBAL, nombres[i]);
        fprintf(stdout, "[fumador %s] abriendo semaforo %s\r\n", id_fumador, real);
        e->hSems[i] = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, real);
        if (e->hSems[i] == NULL) {
            fprintf(stderr, "Error abriendo semaforo %s (%lu)\r\n", real, GetLastError());
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
        fprintf(stdout, "[Fumador %s] esperando\r\n", e->id_fumador);
        WaitForSingleObject(e->hSems[SEM_FUMADOR_IDX], INFINITE);
        fprintf(stdout, "[Fumador %s] tomo ingredientes, lia y fuma\r\n", e->id_fumador);
        Sleep(rand() % MAX_TIEMPO_FUMANDO + 1);
        ReleaseSemaphore(e->hSems[SEM_AGENTE_IDX], 1, NULL);
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

    srand((unsigned) (time(NULL) ^ GetCurrentProcessId()));

    PEstadoFumador estado = crear_estado_fumador(nom_agente, nom_fumador, id_fumador);
    if (estado == NULL) ExitProcess(EXIT_FAILURE);
    g_estado = estado;

    SetConsoleCtrlHandler(handle_signals, TRUE);
    ejecutar_fumador(estado);
    ExitProcess(EXIT_SUCCESS);
}
