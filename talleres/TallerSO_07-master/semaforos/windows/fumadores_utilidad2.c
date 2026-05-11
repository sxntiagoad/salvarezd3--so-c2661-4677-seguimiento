#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fumadores2.h"

char* obtener_nombre_semaforo(char const *prefijo, char const *nom_sem) {
    size_t tam = strlen(prefijo) + strlen(nom_sem) + 1;
    char *r = malloc(tam);
    r[0] = '\0';
    strcpy(r, prefijo);
    strcat(r, nom_sem);
    return r;
}

PEstadoAgente crear_estado_agente(char const *nom_sems[]) {
    PEstadoAgente e = malloc(sizeof(EstadoAgente));
    if (e == NULL) return NULL;
    ZeroMemory(e, sizeof(EstadoAgente));

    for (int i = 0; i < NUM_SEMS; i++) {
        e->nom_real_sem[i] = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);
        fprintf(stdout, "[Agente] Creando semaforo: %s\r\n", e->nom_real_sem[i]);

        e->hSems[i] = CreateSemaphore(NULL, 0, 1, e->nom_real_sem[i]);
        if (e->hSems[i] == NULL) {
            fprintf(stderr, "Error: creando semaforo %lu\r\n", GetLastError());
            for (int j = 0; j < i; j++) {
                CloseHandle(e->hSems[j]);
                free(e->nom_real_sem[j]);
            }
            free(e->nom_real_sem[i]);
            free(e);
            return NULL;
        }
    }
    return e;
}

void destruir_estado_agente(PEstadoAgente e) {
    if (e == NULL) return;
    for (int i = 0; i < NUM_SEMS; i++) {
        if (e->hSems[i] != NULL) CloseHandle(e->hSems[i]);
        free(e->nom_real_sem[i]);
    }
    free(e);
}

PEstadoControl crear_estado_control(int num_procesos) {
    PEstadoControl e = malloc(sizeof(EstadoControl));
    if (e == NULL) return NULL;
    e->num_procesos = num_procesos;
    e->pi = calloc(num_procesos, sizeof(PROCESS_INFORMATION));
    if (e->pi == NULL) { free(e); return NULL; }
    return e;
}

void destruir_estado_control(PEstadoControl e) {
    if (e == NULL) return;
    for (int i = 0; i < e->num_procesos; i++) {
        if (e->pi[i].hProcess) CloseHandle(e->pi[i].hProcess);
        if (e->pi[i].hThread)  CloseHandle(e->pi[i].hThread);
    }
    free(e->pi);
    free(e);
}
