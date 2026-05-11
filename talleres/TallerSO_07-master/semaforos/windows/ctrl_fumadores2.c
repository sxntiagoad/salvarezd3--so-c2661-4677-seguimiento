#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fumadores2.h"

static PEstadoControl g_control = NULL;

static BOOL WINAPI handle_signals(DWORD dwCtrlType) {
    (void) dwCtrlType;
    fprintf(stdout, "[ctrl_fumadores2] Terminando procesos\r\n");
    if (g_control != NULL) {
        for (int i = 0; i < g_control->num_procesos; i++) {
            if (g_control->pi[i].hProcess) {
                TerminateProcess(g_control->pi[i].hProcess, EXIT_SUCCESS);
            }
        }
    }
    return TRUE;
}

static void usage(char const *prog, DWORD code) {
    fprintf(stderr,
            "Uso: %s [-a <nom>] [-c <nom>] [-p <nom>] [-t <nom>]\r\n"
            "     %s -h\r\n", prog, prog);
    ExitProcess(code);
}

static int lanzar_proceso(PROCESS_INFORMATION *pi, char *linea_comando) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(pi, sizeof(PROCESS_INFORMATION));

    return CreateProcess(NULL, linea_comando, NULL, NULL, FALSE,
                         0, NULL, NULL, &si, pi);
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
    if (control == NULL) ExitProcess(EXIT_FAILURE);
    g_control = control;

    char linea_comando[1024];

    sprintf(linea_comando, "./agente2 -a %s -c %s -p %s -t %s",
            nom_sems[AGENTE], nom_sems[CERILLA], nom_sems[PAPEL], nom_sems[TABACO]);
    if (!lanzar_proceso(&control->pi[0], linea_comando)) {
        fprintf(stderr, "Error lanzando agente2: %lu\r\n", GetLastError());
        ExitProcess(EXIT_FAILURE);
    }

    Sleep(MAX_TIEMPO_PREPARANDO);

    for (int i = 0; i < AGENTE; i++) {
        sprintf(linea_comando, "./fumadores2 -a %s -f %s",
                nom_sems[AGENTE], nom_sems[i]);
        if (!lanzar_proceso(&control->pi[i+1], linea_comando)) {
            fprintf(stderr, "Error lanzando fumador %d: %lu\r\n", i, GetLastError());
        }
    }

    SetConsoleCtrlHandler(handle_signals, TRUE);

    /* Espera a que terminen */
    HANDLE handles[NUM_SEMS];
    for (int i = 0; i < NUM_SEMS; i++) handles[i] = control->pi[i].hProcess;
    WaitForMultipleObjects(NUM_SEMS, handles, TRUE, INFINITE);

    destruir_estado_control(control);
    ExitProcess(EXIT_SUCCESS);
}
