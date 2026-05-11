#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <semaphore.h>
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

    for (int i = 0; i < NUM_SEMS; i++) {
        e->nom_real_sem[i] = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);
        fprintf(stdout, "[Agente] Creando semaforo: %s\n", e->nom_real_sem[i]);

        e->sems[i] = sem_open(e->nom_real_sem[i],
                              O_CREAT | O_EXCL,
                              S_IRUSR | S_IWUSR,
                              0);

        if (e->sems[i] == SEM_FAILED) {
            fprintf(stderr, "Error: creando semaforo (%d)\n", errno);
            for (int j = 0; j < i; j++) {
                sem_close(e->sems[j]);
                sem_unlink(e->nom_real_sem[j]);
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
        sem_close(e->sems[i]);
        sem_unlink(e->nom_real_sem[i]);
        free(e->nom_real_sem[i]);
    }
    free(e);
}

PEstadoControl crear_estado_control(int num_procesos) {
    PEstadoControl e = malloc(sizeof(EstadoControl));
    if (e == NULL) return NULL;
    e->num_procesos = num_procesos;
    e->procesos     = calloc(num_procesos, sizeof(int));
    if (e->procesos == NULL) { free(e); return NULL; }
    return e;
}

void destruir_estado_control(PEstadoControl e) {
    if (e == NULL) return;
    free(e->procesos);
    free(e);
}
